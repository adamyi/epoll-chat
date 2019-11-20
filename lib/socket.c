#include <fcntl.h>
#include <netinet/in.h>
#include <poll.h>
#include <pthread.h>
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

im_client_t *im_connection_accept(int epollfd, int sockfd,
                                  struct sockaddr_in clientaddr) {
  ac_log(AC_LOG_DEBUG, "new connection, sockfd: %d", sockfd);
  im_client_t *client = malloc(sizeof(im_client_t));
  memset(client, 0, sizeof(im_client_t));
  client->fd = sockfd;
  client->close_callback = NULL;
  client->close_prehook = NULL;
  client->addr = clientaddr;
  client->nchunks = 0;
  client->chunkids = NULL;
  init_buffer(&(client->outbuffer), OUT_BUFFER_DEFAULT_SIZE);
  init_buffer(&(client->inbuffer), MSG_LIMIT);

  struct epoll_event event;
  event.data.fd = sockfd;
  event.events = EPOLLIN;
  if (epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &event) < 0) {
    ac_log(AC_LOG_ERROR, "Couldn't add socket: %s", strerror(errno));
  }

  if (pthread_mutex_init(&(client->lock), NULL) != 0) {
    ac_log(AC_LOG_ERROR, "pthread_mutex_init failed");
  }
  return client;
}

void send_response(im_buffer_t *buffer, struct IMResponse *msg) {
  size_t len;
  uint8_t *bytes = encodeIMResponseToBytes(msg, &len);
  ac_log(AC_LOG_INFO, "sending response to buf %p, len: %u", buffer, len);
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
  pthread_mutex_lock(&(client->lock));
  send_response(&(client->outbuffer), msg);
  struct epoll_event event;
  event.data.fd = client->fd;
  event.events = EPOLLIN | EPOLLOUT;
  if (epoll_ctl(epollfd, EPOLL_CTL_MOD, client->fd, &event) < 0) {
    ac_log(AC_LOG_ERROR, "Couldn't listen on output events for socket: %s",
           strerror(errno));
  }
  ac_log(AC_LOG_DEBUG, "listening on EPOLLIN and EPOLLOUT for %d",
         event.data.fd);
  pthread_mutex_unlock(&(client->lock));
}

void send_response_to_user(UserDb *db, int epollfd, user_t *user,
                           struct IMResponse *msg) {
  ac_log(AC_LOG_DEBUG, "send response to user");
  if (user->client != NULL && isUserLoggedIn(db, user)) {
    ac_log(AC_LOG_DEBUG, "send_response_to_client");
    send_response_to_client(epollfd, user->client, msg);
  } else {
    ac_log(AC_LOG_DEBUG, "send_response");
    send_response(&(user->buffer), msg);
  }
}

void close_socket(int epollfd, UserDb *db, im_client_t *client) {
  if (client == NULL) return;
  pthread_mutex_lock(&(client->lock));
  if (client->close_prehook != NULL) client->close_prehook(client);
  ac_log(AC_LOG_INFO, "closing socket %d", client->fd);
  logoutUser(db, epollfd, client->user);
  if (epoll_ctl(epollfd, EPOLL_CTL_DEL, client->fd, NULL) < 0) {
    ac_log(AC_LOG_ERROR, "error closing socket %d", client->fd);
  }
  close(client->fd);
  pthread_mutex_unlock(&(client->lock));
  if (client->close_callback != NULL) client->close_callback(client);
  return;
}

void im_receive_command(
    int epollfd, UserDb *db, im_client_t *client, struct epoll_event *event,
    size_t (*handler)(UserDb *db, int epollfd, im_client_t *client,
                      uint8_t *cmd, size_t len, struct IMResponse **rsp)) {
  ac_log(AC_LOG_DEBUG, "im_receive_command");
  reset_buffer_start(&(client->inbuffer));
  if ((client->inbuffer.buffer_end << 1) > client->inbuffer.buffer_capacity) {
    client->inbuffer.buffer_capacity <<= 1;
    client->inbuffer.buffer =
        realloc(client->inbuffer.buffer, client->inbuffer.buffer_capacity);
    ac_log(AC_LOG_DEBUG, "resizing receiving buffer to %u",
           client->inbuffer.buffer_capacity);
  }
  size_t nbytes = recv(
      event->data.fd, client->inbuffer.buffer + client->inbuffer.buffer_end,
      client->inbuffer.buffer_capacity - client->inbuffer.buffer_end, 0);
  if (nbytes == 0) {  // disconnect
    close_socket(epollfd, db, client);
    return;
  }
  client->inbuffer.buffer_end += nbytes;
  // ac_log(AC_LOG_DEBUG, "%s", client->inbuffer.buffer +
  // client->inbuffer.buffer_start);
  while (client->inbuffer.buffer_start != client->inbuffer.buffer_end) {
    struct IMResponse *rsp = NULL;
    size_t parsed = handler(
        db, epollfd, client,
        client->inbuffer.buffer + client->inbuffer.buffer_start,
        client->inbuffer.buffer_end - client->inbuffer.buffer_start, &rsp);
    if (rsp != NULL) send_response_to_client(epollfd, client, rsp);
    if (parsed == 0) break;
    client->inbuffer.buffer_start += parsed;
  }
}

void im_send_buffer(int epollfd, UserDb *db, im_client_t *client,
                    im_buffer_t *buffer) {
  ac_log(AC_LOG_DEBUG, "im_send_buffer");
  size_t len = buffer->buffer_end - buffer->buffer_start;
  ac_log(AC_LOG_INFO, "to send: %d bytes to %p", len, buffer);
  if (len > 0) {
    int nsent = send(client->fd, buffer->buffer + buffer->buffer_start, len, 0);
    ac_log(AC_LOG_INFO, "sent: %d bytes", nsent);
    if (nsent == -1) {
      ac_log(AC_LOG_ERROR, "error sending: %s", strerror(errno));
      return;
    }
    buffer->buffer_start += nsent;
    len -= nsent;
  }
  if (len == 0) {
    struct epoll_event event;
    event.data.fd = client->fd;
    event.events = EPOLLIN;
    if (epoll_ctl(epollfd, EPOLL_CTL_MOD, client->fd, &event) < 0) {
      ac_log(AC_LOG_ERROR, "Couldn't listen on input events for socket: %s",
             strerror(errno));
    }
  }
  if ((buffer->buffer_start << 1) >= buffer->buffer_capacity)
    reset_buffer_start(buffer);
}

void free_client(im_client_t *client) {
  free(client->inbuffer.buffer);
  free(client->outbuffer.buffer);
  free(client);
}

int pick_client(struct im_client *clients[], int sockfd, int *nclients,
                bool createNew) {
  for (int i = 0; i < *nclients; i++) {
    if (clients[i]->fd == sockfd) {
      if (createNew) free_client(clients[i]);
      return i;
    }
  }
  if (createNew) return (*nclients)++;
  return -1;
}

// TODO: implement generalized [packed repeated value] (en/de)coding in
// ac_protobuf
uint32_t **parse_packed_uint32(ac_protobuf_string_t *bytes, size_t *n) {
  size_t len = bytes->len >> 2;
  uint32_t **ret = malloc((len + 1) * sizeof(void *));
  uint32_t **curr = ret;
  uint32_t *decodeptr = (uint32_t *)bytes->value;
  for (int i = 0; i < len; ++i, ++curr, ++decodeptr) {
    *curr = malloc(4);
    **curr = *decodeptr;
  }
  *curr = NULL;
  if (n != NULL) *n = len;
  return ret;
}

size_t pack_repeated_uint32_from_str(char *str, ac_protobuf_string_t *bytes) {
  bytes->len = 0;
  uint32_t *vals = malloc(sizeof(uint32_t) * (1 + strlen(str) / 2));
  size_t ret = 0;
  char *s = str;
  char *e;
  while (true) {
    e = NULL;
    vals[ret] = strtol(s, &e, 10);
    if (e == s) {
      ac_log(AC_LOG_ERROR,
             "Error: invalid character in chunk sequence in command");
      free(vals);
      return 0;
    }
    ret++;
    if (*e == '\0') {
      break;
    }
    s = e;
  }
  bytes->len = ret << 2;
  bytes->value = malloc(bytes->len);
  uint32_t *rvals = (uint32_t *)bytes->value;
  for (size_t i = 0; i < ret; i++) {
    rvals[i] = vals[i];
  }
  free(vals);
  return ret;
}

void free_parsed_packed_uint32(uint32_t **values) {
  uint32_t **curr = values;
  while (*curr != NULL) {
    free(*curr);
    curr++;
  }
}
