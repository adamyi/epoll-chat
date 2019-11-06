#ifndef IM_COMMAND_H_
#define IM_COMMAND_H_

#include "client.h"
// do not sort
#include "auth.h"

#include "proto/IMResponse.pb.h"

typedef struct im_command {
  uint32_t type;
  struct IMResponse *(*run)(UserDb *db, struct im_client *client, void *req);
  void *(*parser)(uint8_t *bytes, size_t len);
  void (*freeer)(void *val);
} im_command_t;

int parse_command(UserDb *db, struct im_client *client, uint8_t *cmd,
                  size_t len, struct IMResponse **rsp);

extern const im_command_t *enabled_commands[];

#endif
