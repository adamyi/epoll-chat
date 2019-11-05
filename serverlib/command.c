#include "achelper/ac_log.h"
#include "achelper/ac_protobuf.h"

#include "client.h"
// do not sort
#include "auth.h"

#include "serverlib/commands/login.h"

const im_command_t *enabled_commands[] = {&cmd_login, NULL};

void parse_command(UserDb *db, struct im_client *client, uint8_t *cmd,
                   size_t len) {
  ac_protobuf_message_t *msg = ac_decode_protobuf_msg(cmd, len);
  ac_protobuf_field_t *type = ac_find_protobuf_field_in_msg(msg, 1);
  if (type == NULL) {
    ac_log(AC_LOG_ERROR,
           "protobuf decode failure: request type does not exist");
    return;
  }
  ac_protobuf_field_t *content = ac_find_protobuf_field_in_msg(msg, 2);
  if (content == NULL) {
    ac_log(AC_LOG_ERROR,
           "protobuf decode failure: request type does not exist");
    return;
  }
  ac_protobuf_print_msg(msg);
  ac_protobuf_message_t *req =
      ac_decode_protobuf_msg(content->value, content->len);
  uint64_t reqtype = *((uint64_t *)type->value);
  bool hasrun = false;
  for (const im_command_t **cmd = enabled_commands; cmd != NULL; cmd++) {
    if ((*cmd)->type == reqtype) {
      ac_log(AC_LOG_DEBUG, "found implementation for request type %lu",
             reqtype);
      hasrun = true;
      (*cmd)->run(db, client, req);
      break;
    }
  }
  if (!hasrun)
    ac_log(AC_LOG_ERROR, "protobuf decode failure: request type %lu unknown",
           reqtype);
}
