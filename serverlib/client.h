#ifndef IM_CLIENT_H_
#define IM_CLIENT_H_

#include "achelper/ac_protobuf.h"

#include "auth.h"

#define MSG_LIMIT 32768
#define NAME_LIMIT 256

struct im_client {
  int fd;
  char outbuffer[MSG_LIMIT];
  user_t *user;
};

#endif
