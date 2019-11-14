#ifndef IM_CLIENT_H_
#define IM_CLIENT_H_

#include <netinet/in.h>
#include <pthread.h>
#include <sys/socket.h>

#include "auth.h"

#define MSG_LIMIT 32768
#define NAME_LIMIT 256

typedef struct im_client {
  int fd;
  user_t *user;
  im_buffer_t outbuffer;
  im_buffer_t inbuffer;
  pthread_mutex_t lock;
  void (*close_callback)(struct im_client *client);
  void (*close_prehook)(struct im_client *client);
  struct sockaddr_in addr;
} im_client_t;

#endif
