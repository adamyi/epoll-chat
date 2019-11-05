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

#include "client.h"
// do not sort
#include "auth.h"
#include "command.h"
#include "socket.h"

void make_socket_nonblocking(int sockfd) {
  int flags = fcntl(sockfd, F_GETFL, 0);
  if (flags < 0) {
    fprintf(stderr, "couldn't get socket flags: %s\n", strerror(errno));
    exit(1);
  }
  if (fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) < 0) {
    fprintf(stderr, "couldn't set socket flags: %s\n", strerror(errno));
    exit(1);
  }
}

int listen_socket(int portnum) {
  int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (sockfd < 0) {
    fprintf(stderr, "couldn't create socket: %s\n", strerror(errno));
    exit(1);
  }

  int opt = 1;
  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
    fprintf(stderr, "couldn't setsockopt: %s\n", strerror(errno));
    exit(1);
  }
  struct sockaddr_in listenaddr = {0};
  listenaddr.sin_family = AF_INET;
  listenaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  listenaddr.sin_port = htons(portnum);

  if (bind(sockfd, (struct sockaddr *)&listenaddr, sizeof(listenaddr)) < 0) {
    fprintf(stderr, "couldn't bind: %s\n", strerror(errno));
    exit(1);
  }
  make_socket_nonblocking(sockfd);
  if (listen(sockfd, SOMAXCONN) < 0) {
    fprintf(stderr, "couldn't listen: %s\n", strerror(errno));
    exit(1);
  }
  return sockfd;
}

struct im_client *im_connection_accept(int epollfd, int sockfd) {
  struct im_client *client = malloc(sizeof(struct im_client));
  memset(client, 0, sizeof(struct im_client));
  client->fd = sockfd;

  struct epoll_event event;
  event.data.fd = sockfd;
  event.events = EPOLLIN;
  if (epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &event) < 0) {
    fprintf(stderr, "Couldn't add socket: %s\n", strerror(errno));
  }
  return client;
}

void im_receive_command(UserDb *db, struct im_client *client,
                        struct epoll_event *event) {
  uint8_t buf[MSG_LIMIT];
  size_t nbytes = recv(event->data.fd, buf, sizeof(buf), 0);
  parse_command(db, client, buf, nbytes);
}
