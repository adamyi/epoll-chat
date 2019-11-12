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
#include "achelper/ac_protobuf.h"

#include "client.h"
// do not sort
#include "auth.h"
#include "buffer.h"
#include "command.h"
#include "socket.h"

#include "proto/IMResponse.pb.h"

void make_socket_nonblocking(int sockfd) {
  int flags = fcntl(sockfd, F_GETFL, 0);
  if (flags < 0) {
    ac_log(AC_LOG_FATAL, "couldn't get socket flags: %s", strerror(errno));
  }
  if (fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) < 0) {
    ac_log(AC_LOG_FATAL, "couldn't set socket flags: %s", strerror(errno));
  }
}

int listen_socket(int portnum) {
  int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (sockfd < 0) {
    ac_log(AC_LOG_FATAL, "couldn't create socket: %s", strerror(errno));
  }

  int opt = 1;
  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
    ac_log(AC_LOG_FATAL, "couldn't setsockopt: %s", strerror(errno));
  }
  struct sockaddr_in listenaddr = {0};
  listenaddr.sin_family = AF_INET;
  listenaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  listenaddr.sin_port = htons(portnum);

  if (bind(sockfd, (struct sockaddr *)&listenaddr, sizeof(listenaddr)) < 0) {
    ac_log(AC_LOG_FATAL, "couldn't bind: %s", strerror(errno));
  }
  make_socket_nonblocking(sockfd);
  if (listen(sockfd, SOMAXCONN) < 0) {
    ac_log(AC_LOG_FATAL, "couldn't listen: %s", strerror(errno));
  }
  return sockfd;
}

im_client_t *im_connection_accept(int epollfd, int sockfd) {
  im_client_t *client = malloc(sizeof(im_client_t));
  memset(client, 0, sizeof(im_client_t));
  client->fd = sockfd;
  init_buffer(&(client->outbuffer), OUT_BUFFER_DEFAULT_SIZE);
  init_buffer(&(client->inbuffer), MSG_LIMIT);

  struct epoll_event event;
  event.data.fd = sockfd;
  event.events = EPOLLIN;
  if (epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &event) < 0) {
    fprintf(stderr, "Couldn't add socket: %s\n", strerror(errno));
  }
  return client;
}

void send_response(im_buffer_t *buffer, struct IMResponse *msg) {
  ac_log(AC_LOG_INFO, "sending response");
  size_t len;
  uint8_t *bytes = encodeIMResponseToBytes(msg, &len);
  if (buffer->buffer_end + len >= buffer->buffer_capacity) {
    reset_buffer_start(buffer);
    if (buffer->buffer_end + len >= buffer->buffer_capacity) {
      while (buffer->buffer_end + len >= buffer->buffer_capacity) {
        buffer->buffer_capacity <<= 1;
      }
      buffer->buffer = realloc(buffer->buffer, buffer->buffer_capacity);
    }
  }
  memcpy(buffer->buffer + buffer->buffer_end, bytes, len);
  buffer->buffer_end += len;
}

void send_response_to_client(int epollfd, im_client_t *client,
                             struct IMResponse *msg) {
  ac_log(AC_LOG_DEBUG, "send_response_to_client");
  send_response(&(client->outbuffer), msg);
  struct epoll_event event;
  event.data.fd = client->fd;
  event.events = EPOLLIN | EPOLLOUT;
  if (epoll_ctl(epollfd, EPOLL_CTL_MOD, client->fd, &event) < 0) {
    fprintf(stderr, "Couldn't listen on output events for socket: %s\n",
            strerror(errno));
  }
}

void send_response_to_user(UserDb *db, int epollfd, user_t *user,
                           struct IMResponse *msg) {
  printf("send response to user %s\n", msg->msg.value);
  if (user->client != NULL && isUserLoggedIn(db, user)) {
    printf("send_response_to_client\n");
    send_response_to_client(epollfd, user->client, msg);
  } else {
    printf("send_response\n");
    send_response(&(user->buffer), msg);
  }
}

void im_receive_command(int epollfd, UserDb *db, im_client_t *client,
                        struct epoll_event *event) {
  reset_buffer_start(&(client->inbuffer));
  size_t nbytes = recv(
      event->data.fd, client->inbuffer.buffer + client->inbuffer.buffer_start,
      client->inbuffer.buffer_capacity - client->inbuffer.buffer_start, 0);
  client->inbuffer.buffer_end += nbytes;
  struct IMResponse *rsp = NULL;
  // ac_log(AC_LOG_DEBUG, "%s", client->inbuffer.buffer +
  // client->inbuffer.buffer_start);
  client->inbuffer.buffer_start += parse_command(
      db, epollfd, client, client->inbuffer.buffer,
      client->inbuffer.buffer_end - client->inbuffer.buffer_start, &rsp);
  if (rsp != NULL) {
    send_response_to_client(epollfd, client, rsp);
  }
}

void im_send_buffer(int epollfd, UserDb *db, im_client_t *client,
                    im_buffer_t *buffer, struct epoll_event *event) {
  printf("im_send_buffer\n");
  size_t len = buffer->buffer_end - buffer->buffer_start;
  if (len > 0) {
    ac_log(AC_LOG_INFO, "to send: %d bytes", len);
    int nsent = send(client->fd, buffer->buffer + buffer->buffer_start, len, 0);
    ac_log(AC_LOG_INFO, "sent: %d bytes", nsent);
    if (nsent == -1) {
      fprintf(stderr, "error sending: %s\n", strerror(errno));
      return;
    }
    buffer->buffer_start += nsent;
    len -= nsent;
  }
  if (len == 0) {
    struct epoll_event nevent;
    nevent.data.fd = client->fd;
    nevent.events = EPOLLIN;
    if (epoll_ctl(epollfd, EPOLL_CTL_MOD, client->fd, &nevent) < 0) {
      fprintf(stderr, "Couldn't listen on input events for socket: %s\n",
              strerror(errno));
    }
  }
  if ((buffer->buffer_start << 1) >= buffer->buffer_capacity)
    reset_buffer_start(buffer);
}
