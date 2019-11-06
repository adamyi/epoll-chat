#ifndef IM_CLIENT_H_
#define IM_CLIENT_H_

#include "auth.h"

#define MSG_LIMIT 32768
#define NAME_LIMIT 256

#define OUT_BUFFER_DEFAULT_SIZE BUFSIZ

struct im_client {
  int fd;
  uint8_t *outbuffer;
  size_t buffer_capacity;
  size_t buffer_start;
  size_t buffer_end;
  user_t *user;
};

#endif
