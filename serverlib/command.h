#ifndef IM_COMMAND_H_
#define IM_COMMAND_H_

#include "client.h"
// do not sort
#include "auth.h"

typedef struct im_command {
  uint32_t type;
  void (*run)(UserDb *db, struct im_client *client, void *req);
  void *(*parser)(uint8_t *bytes, size_t len);
  void (*freeer)(void *val);
} im_command_t;

void parse_command(UserDb *db, struct im_client *client, uint8_t *cmd,
                   size_t len);

extern const im_command_t *enabled_commands[];

#endif
