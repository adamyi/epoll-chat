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

#include "achelper/ac_log.h"
#include "achelper/ac_protobuf.h"

#include "client.h"
// do not sort
#include "auth.h"
#include "command.h"
#include "socket.h"

#include "proto/IMResponse.pb.h"

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
  client->outbuffer = malloc(OUT_BUFFER_DEFAULT_SIZE);
  client->buffer_capacity = OUT_BUFFER_DEFAULT_SIZE;
  client->buffer_end = 0;
  client->buffer_start = 0;

  struct epoll_event event;
  event.data.fd = sockfd;
  event.events = EPOLLIN;
  if (epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &event) < 0) {
    fprintf(stderr, "Couldn't add socket: %s\n", strerror(errno));
  }
  return client;
}

void reset_buffer_start(struct im_client *client) {
  if (client->buffer_start == 0) return;
  memcpy(client->outbuffer, client->outbuffer + client->buffer_start,
         client->buffer_end - client->buffer_start);
  client->buffer_end -= client->buffer_start;
  client->buffer_start = 0;
}

void send_response(int epollfd, struct im_client *client,
                   struct IMResponse *msg) {
  ac_log(AC_LOG_INFO, "sending response");
  size_t len;
  uint8_t *bytes = encodeIMResponseToBytes(msg, &len);
  if (client->buffer_end + len >= client->buffer_capacity) {
    reset_buffer_start(client);
    if (client->buffer_end + len >= client->buffer_capacity) {
      while (client->buffer_end + len >= client->buffer_capacity) {
        client->buffer_capacity <<= 1;
      }
      client->outbuffer = realloc(client->outbuffer, client->buffer_capacity);
    }
  }
  memcpy(client->outbuffer + client->buffer_end, bytes, len);
  client->buffer_end += len;
  struct epoll_event event;
  event.data.fd = client->fd;
  event.events = EPOLLIN | EPOLLOUT;
  if (epoll_ctl(epollfd, EPOLL_CTL_MOD, client->fd, &event) < 0) {
    fprintf(stderr, "Couldn't listen on output events for socket: %s\n",
            strerror(errno));
  }
}

void im_receive_command(int epollfd, UserDb *db, struct im_client *client,
                        struct epoll_event *event) {
  uint8_t buf[MSG_LIMIT];
  size_t nbytes = recv(event->data.fd, buf, sizeof(buf), 0);
  struct IMResponse *rsp;
  int r = parse_command(db, client, buf, nbytes, &rsp);
  if (r == 0) {
    if (rsp != NULL) {
      send_response(epollfd, client, rsp);
    }
  } else if (r == 1) {
    // TODO: wait for further input
  }
}

void im_send_buffer(int epollfd, UserDb *db, struct im_client *client,
                    struct epoll_event *event) {
  size_t len = client->buffer_end - client->buffer_start;
  if (len > 0) {
    ac_log(AC_LOG_INFO, "to send: %d bytes", len);
    int nsent =
        send(client->fd, client->outbuffer + client->buffer_start, len, 0);
    ac_log(AC_LOG_INFO, "sent: %d bytes", nsent);
    if (nsent == -1) {
      fprintf(stderr, "error sending: %s\n", strerror(errno));
      return;
    }
    client->buffer_start += nsent;
    len -= nsent;
  }
  if (len == 0) {
    struct epoll_event event;
    event.data.fd = client->fd;
    event.events = EPOLLIN;
    if (epoll_ctl(epollfd, EPOLL_CTL_MOD, client->fd, &event) < 0) {
      fprintf(stderr, "Couldn't listen on input events for socket: %s\n",
              strerror(errno));
    }
  }
  if ((client->buffer_start << 1) >= client->buffer_capacity)
    reset_buffer_start(client);
}
