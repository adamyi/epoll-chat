#ifndef IM_COMMAND_H_
#define IM_COMMAND_H_

#define REQUEST_LOGIN 1

#include "achelper/ac_protobuf.h"

#include "client.h"
// do not sort
#include "auth.h"

typedef struct im_command {
  uint32_t type;
  void (*run)(UserDb *db, struct im_client *client, ac_protobuf_message_t *req);
} im_command_t;

void parse_command(UserDb *db, struct im_client *client, uint8_t *cmd,
                   size_t len);

extern const im_command_t *enabled_commands[];

#endif
