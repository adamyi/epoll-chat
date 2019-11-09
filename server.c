#define _GNU_SOURCE

#include <fcntl.h>
#include <netinet/in.h>
#include <poll.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "achelper/ac_log.h"
#include "achelper/ac_memory.h"
#include "achelper/ac_protobuf.h"

#include "serverlib/client.h"
// do not sort
#include "serverlib/auth.h"
#include "serverlib/socket.h"

#define MAX_EVENTS 16
#define MAX_CLIENTS 32

int main(int argc, char *argv[]) {
  setvbuf(stdout, NULL, _IONBF, 0);
  struct im_client *clients[MAX_CLIENTS] = {NULL};
  int nclients = 0;
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
  UserDb *db = buildUserDb(udbfp, block_duration, timeout);
  fclose(udbfp);

  int masterfd = listen_socket(port);

  int epollfd = epoll_create1(0);
  if (epollfd < 0) {
    ac_log(AC_LOG_FATAL, "couldn't create epoll: %s", strerror(errno));
  }

  struct epoll_event accept_event;
  accept_event.data.fd = masterfd;
  accept_event.events = EPOLLIN | EPOLLET;
  if (epoll_ctl(epollfd, EPOLL_CTL_ADD, masterfd, &accept_event) < 0) {
    ac_log(AC_LOG_FATAL, "epoll_ctl EPOLL_CTL_ADD error: %s", strerror(errno));
  }

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
        clients[nclients++] = im_connection_accept(epollfd, newsockfd);
      } else {
        if (events[i].events & EPOLLIN) {
          for (int i = 0; i < nclients; i++) {
            if (clients[i]->fd == events[i].data.fd) {
              im_receive_command(epollfd, db, clients[i], events + i);
              break;
            }
          }
        }
        if (events[i].events & EPOLLOUT) {
          printf("epollout\n");
          for (int i = 0; i < nclients; i++) {
            if (clients[i]->fd == events[i].data.fd) {
              im_send_buffer(epollfd, db, clients[i], &(clients[i]->outbuffer),
                             events + i);
              int ct = (int)time(NULL);
              if (clients[i]->user != NULL &&
                  clients[i]->user->last_active + db->login_timeout > ct)
                im_send_buffer(epollfd, db, clients[i],
                               &(clients[i]->user->buffer), events + i);
              break;
            }
          }
        }
      }
    }
  }
}
