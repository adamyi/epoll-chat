#define _GNU_SOURCE

#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <poll.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include "achelper/ac_log.h"
#include "achelper/ac_memory.h"
#include "achelper/ac_protobuf.h"

#include "lib/client.h"
// do not sort
#include "lib/auth.h"
#include "lib/socket.h"

#include "clientlib/command.h"

#include "proto/ExitResponse.pb.h"
#include "proto/GetIPResponse.pb.h"
#include "proto/IMResponse.pb.h"
#include "proto/TextResponse.pb.h"

#define MAX_EVENTS 16
#define MAX_CLIENTS 32

#define MAX_COMMAND 32768

struct im_client *clients[MAX_CLIENTS] = {NULL};
int nclients = 0;
int epollfd;
int masterfd;

uint32_t listen_port = 0;

bool lastresponse = false;
pthread_mutex_t loginlock;

size_t parse_response(UserDb *db, int epollfd, im_client_t *client,
                      uint8_t *cmd, size_t len, struct IMResponse **rsp) {
  size_t ret = 0;
  ac_protobuf_message_t *msg =
      ac_decode_protobuf_msg_with_n_fields(cmd, len, 3, &ret);
  if (msg == NULL) {
    ac_log(AC_LOG_ERROR, "protobuf decode failure: invalid protobuf");
    return 0;
  }
  struct IMResponse *imrsp = parseIMResponseFromProtobufMsg(msg);
  lastresponse = imrsp->success;

  ac_protobuf_print_msg(msg);
  size_t read = 0;
  switch (imrsp->type) {
    case 1:;
      struct TextResponse *tr = parseTextResponseFromBytes(
          imrsp->value.value, imrsp->value.len, &read);
      printf("%s\n", tr->msg.value);
      freeTextResponse(tr);
      break;
    case 2:;
      struct ExitResponse *er = parseExitResponseFromBytes(
          imrsp->value.value, imrsp->value.len, &read);
      printf("%s\n", er->msg.value);
      freeExitResponse(er);
      exit(0);
      break;
    case 3:;
      struct GetIPResponse *gr = parseGetIPResponseFromBytes(
          imrsp->value.value, imrsp->value.len, &read);
      struct sockaddr_in client_addr;
      memcpy(&client_addr, gr->addr.value, sizeof(struct sockaddr_in));
      char addr[100];
      inet_ntop(AF_INET, &(client_addr.sin_addr), addr, 100);
      ac_log(AC_LOG_DEBUG, "Got private addr %s:%u", addr,
             ntohs(client_addr.sin_port));
      freeGetIPResponse(gr);
      break;
    default:
      ac_log(AC_LOG_ERROR, "unidentified response type %u", imrsp->type);
  }
  freeIMResponse(imrsp);
  return ret;
}

static void got_command(char *buf, size_t l) {
  parse_command(epollfd, clients[0], (uint8_t *)buf, l);
  ac_log(AC_LOG_DEBUG, "after parse");
  struct epoll_event event;
  event.data.fd = clients[0]->fd;
  event.events = EPOLLIN | EPOLLOUT;
  if (epoll_ctl(epollfd, EPOLL_CTL_MOD, clients[0]->fd, &event) < 0) {
    fprintf(stderr, "Couldn't listen on output events for socket: %s\n",
            strerror(errno));
  }
}

void *input_thread(void *arg) {
  while (true) {
    pthread_mutex_lock(&loginlock);
    if (lastresponse) break;
    got_command("login", 5);
  }
  char buf[MAX_COMMAND];
  while (true) {
    if (fgets(buf, MAX_COMMAND, stdin) != buf) continue;
    size_t l = strlen(buf);
    buf[--l] = '\0';  // remove ending \n
    ac_log(AC_LOG_DEBUG, "got command: %s (%u)", buf, l);
    got_command(buf, l);
  }
  return NULL;
}

void *network_thread(void *arg) {
  struct epoll_event *events =
      ac_malloc(MAX_EVENTS * sizeof(struct epoll_event), "epoll events");
  memset(events, 0, MAX_EVENTS * sizeof(struct epoll_event));

  while (true) {
    int N = epoll_wait(epollfd, events, MAX_EVENTS, -1);
    for (int i = 0; i < N; i++) {
      if (events[i].events & (EPOLLERR | EPOLLHUP | EPOLLRDHUP)) {
        fprintf(stderr, "disconnect\n");
        // TODO: disconnect
      } else if (events[i].data.fd == masterfd) {
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        int newsockfd =
            accept(masterfd, (struct sockaddr *)&client_addr, &client_addr_len);
        if (newsockfd < 0) {
          fprintf(stderr, "couldn't accept connection\n");
          continue;
        }
        make_socket_nonblocking(newsockfd);
        clients[nclients++] =
            im_connection_accept(epollfd, newsockfd, client_addr);
      } else {
        if (events[i].events & EPOLLIN) {
          for (int j = 0; j < nclients; j++) {
            if (clients[j]->fd == events[i].data.fd) {
              printf("received command\n");
              im_receive_command(epollfd, NULL, clients[j], events + i,
                                 parse_response);
              pthread_mutex_unlock(&loginlock);
              break;
            }
          }
        }
        if (events[i].events & EPOLLOUT) {
          printf("epollout\n");
          for (int j = 0; j < nclients; j++) {
            if (clients[j]->fd == events[i].data.fd) {
              pthread_mutex_lock(&(clients[j]->lock));
              im_send_buffer(epollfd, NULL, clients[j],
                             &(clients[j]->outbuffer));
              pthread_mutex_unlock(&(clients[j]->lock));
              break;
            }
          }
        }
      }
    }
  }
  return NULL;
}

void server_close_connection() {
  ac_log(AC_LOG_FATAL, "Server has closed connection");
}

int main(int argc, char *argv[]) {
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);

  if (argc < 3) {
    ac_log(AC_LOG_FATAL, "Usage: ./client server_ip server_port");
  }
  char *server_name = argv[1];
  int server_port = atoi(argv[2]);

  struct sockaddr_in server_address;
  memset(&server_address, 0, sizeof(server_address));
  server_address.sin_family = AF_INET;

  inet_pton(AF_INET, server_name, &server_address.sin_addr);

  server_address.sin_port = htons(server_port);

  // open socket
  int sock;
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    ac_log(AC_LOG_FATAL, "Could not open socket");
  }

  // 3 way handshake
  if (connect(sock, (struct sockaddr *)&server_address,
              sizeof(server_address)) < 0) {
    ac_log(AC_LOG_FATAL, "Could not connect to server");
    return 1;
  }

  masterfd = listen_socket(0);
  struct sockaddr_in listenaddr = {0};
  unsigned int listenaddr_len = sizeof(listenaddr);
  if (getsockname(masterfd, &listenaddr, &listenaddr_len) < 0) {
    ac_log(AC_LOG_FATAL, "couldn't listen on local ephemeral port: %s",
           strerror(errno));
  }
  listen_port = ntohs(listenaddr.sin_port);
  ac_log(AC_LOG_INFO, "listening on port %u", listen_port);

  epollfd = epoll_create1(0);
  if (epollfd < 0) {
    ac_log(AC_LOG_FATAL, "couldn't create epoll: %s", strerror(errno));
  }

  struct epoll_event accept_event;
  accept_event.data.fd = masterfd;
  accept_event.events = EPOLLIN | EPOLLET;
  if (epoll_ctl(epollfd, EPOLL_CTL_ADD, masterfd, &accept_event) < 0) {
    ac_log(AC_LOG_FATAL, "epoll_ctl EPOLL_CTL_ADD error: %s", strerror(errno));
  }

  clients[nclients] = im_connection_accept(epollfd, sock, server_address);
  clients[nclients]->close_callback = server_close_connection;
  nclients++;

  pthread_t nt, it;
  if (pthread_mutex_init(&loginlock, NULL) != 0) {
    ac_log(AC_LOG_FATAL, "cannot lock loginlock");
  }
  pthread_create(&nt, NULL, &network_thread, NULL);
  pthread_create(&it, NULL, &input_thread, NULL);
  pthread_join(nt, NULL);
  pthread_join(it, NULL);
}
