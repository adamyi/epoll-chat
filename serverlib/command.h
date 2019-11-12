#ifndef IM_COMMAND_H_
#define IM_COMMAND_H_

#include "lib/client.h"
// do not sort
#include "lib/auth.h"

#include "proto/IMResponse.pb.h"

typedef struct im_command {
  uint32_t type;
  struct IMResponse *(*run)(UserDb *db, int epollfd, im_client_t *client,
                            void *req);
  void *(*parser)(uint8_t *bytes, size_t len, size_t *readbytes);
  void (*freeer)(void *val);
} im_command_t;

size_t parse_command(UserDb *db, int epollfd, im_client_t *client, uint8_t *cmd,
                     size_t len, struct IMResponse **rsp);

extern const im_command_t *enabled_commands[];

#endif
