#ifndef IM_CLIENT_H_
#define IM_CLIENT_H_

#include <pthread.h>

#include "auth.h"

#define MSG_LIMIT 32768
#define NAME_LIMIT 256

typedef struct im_client {
  int fd;
  user_t *user;
  im_buffer_t outbuffer;
  im_buffer_t inbuffer;
  pthread_mutex_t lock;
  void (*close_callback)();
} im_client_t;

#endif
