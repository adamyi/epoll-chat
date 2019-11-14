#define _GNU_SOURCE

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

#include "serverlib/command.h"

#define MAX_EVENTS 16
#define MAX_CLIENTS 32

#define LOUGOUT_CHECK_INTERVAL 1

bool IS_SERVER = true;

struct im_client *clients[MAX_CLIENTS] = {NULL};
int nclients = 0;
int epollfd;
int masterfd;

UserDb *db;

void *time_thread(void *arg) {
  struct IMResponse *rsp = encodeExitTextToIMResponse(
      "You haven't issued any command for a while. You are automatically "
      "logged off.",
      false);
  while (true) {
    sleep(LOUGOUT_CHECK_INTERVAL);
    for (int i = 0; i < nclients; i++) {
      if (clients[i]->user != NULL && (!isUserLoggedIn(db, clients[i]->user))) {
        logoutUser(db, epollfd, clients[i]->user);
        send_response_to_client(epollfd, clients[i], rsp);
      }
    }
  }
  return NULL;
}

void *network_thread(void *arg) {
  struct epoll_event *events =
      ac_malloc(MAX_EVENTS * sizeof(struct epoll_event), "epoll events");
  memset(events, 0, MAX_EVENTS * sizeof(struct epoll_event));

  while (true) {
    int N = epoll_wait(epollfd, events, MAX_EVENTS, -1);
    ac_log(AC_LOG_DEBUG, "epoll_wait returned %d", N);
    for (int i = 0; i < N; i++) {
      if (events[i].events & (EPOLLERR | EPOLLHUP | EPOLLRDHUP)) {
        fprintf(stderr, "disconnect\n");
        // TODO: disconnect
      } else if (events[i].data.fd == masterfd) {
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        int newsockfd =
            accept(masterfd, (struct sockaddr *)&client_addr, &client_addr_len);
        ac_log(AC_LOG_DEBUG, "new socket %d", newsockfd);
        if (newsockfd < 0) {
          fprintf(stderr, "couldn't accept connection\n");
          continue;
        }
        make_socket_nonblocking(newsockfd);
        clients[pick_client(clients, newsockfd, &nclients, true)] =
            im_connection_accept(epollfd, newsockfd, client_addr);
        ac_log(AC_LOG_DEBUG, "done - new socket %d", newsockfd);
      } else {
        if (events[i].events & EPOLLIN) {
          printf("epollin\n");
          int j = pick_client(clients, events[i].data.fd, &nclients, false);
          if (j < 0) {
            ac_log(AC_LOG_ERROR, "couldn't find im_client for fd %d",
                   events[i].data.fd);
            break;
          }
          if (clients[j]->fd == events[i].data.fd) {
            printf("calling im_receive_command\n");
            im_receive_command(epollfd, db, clients[j], events + i,
                               parse_command);
            break;
          }
        }
        if (events[i].events & EPOLLOUT) {
          printf("epollout\n");
          int j = pick_client(clients, events[i].data.fd, &nclients, false);
          if (j < 0) {
            ac_log(AC_LOG_ERROR, "couldn't find im_client for fd %d",
                   events[i].data.fd);
            break;
          }
          printf("checking %d\n", j);
          if (clients[j]->fd == events[i].data.fd) {
            printf("found match\n");
            pthread_mutex_lock(&(clients[j]->lock));
            im_send_buffer(epollfd, db, clients[j], &(clients[j]->outbuffer));
            printf("aft\n");
            int ct = (int)time(NULL);
            if (clients[j]->user != NULL &&
                clients[j]->user->last_active + db->login_timeout > ct)
              im_send_buffer(epollfd, db, clients[j],
                             &(clients[j]->user->buffer));
            pthread_mutex_unlock(&(clients[j]->lock));
            break;
          }
        }
      }
    }
  }
  return NULL;
}

int main(int argc, char *argv[]) {
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);
  if (argc < 4) {
    ac_log(AC_LOG_FATAL, "usage: ./server server_port block_duration timeout");
  }
  int port = atoi(argv[1]);
  int block_duration = atoi(argv[2]);
  int timeout = atoi(argv[3]);

  FILE *udbfp = fopen("credentials.txt", "r");
  if (udbfp == NULL) {
    ac_log(AC_LOG_FATAL, "couldn't open credentials.txt");
  }
  db = buildUserDb(udbfp, block_duration, timeout);
  fclose(udbfp);

  masterfd = listen_socket(port);

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

  pthread_t nt, tt;
  pthread_create(&nt, NULL, &network_thread, NULL);
  pthread_create(&tt, NULL, &time_thread, NULL);
  pthread_join(nt, NULL);
  pthread_join(tt, NULL);
}
