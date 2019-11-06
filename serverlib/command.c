#include "achelper/ac_log.h"
#include "achelper/ac_protobuf.h"

#include "client.h"
// do not sort
#include "auth.h"

#include "proto/IMRequest.pb.h"
#include "proto/IMResponse.pb.h"

#include "serverlib/commands/login.h"

const im_command_t *enabled_commands[] = {&cmd_login, NULL};

int parse_command(UserDb *db, struct im_client *client, uint8_t *cmd,
                  size_t len, struct IMResponse **rsp) {
  ac_protobuf_message_t *msg =
      ac_decode_protobuf_msg_with_n_fields(cmd, len, 2);
  if (msg == NULL) {
    ac_log(AC_LOG_ERROR, "protobuf decode failure: invalid protobuf");
    return 1;
  }
  struct IMRequest *imreq = parseIMRequestFromProtobufMsg(msg);

  ac_protobuf_print_msg(msg);
  bool hasrun = false;
  for (const im_command_t **cmd = enabled_commands; cmd != NULL; cmd++) {
    if ((*cmd)->type == imreq->type) {
      ac_log(AC_LOG_DEBUG, "found implementation for request type %u",
             imreq->type);
      hasrun = true;
      void *request = (*cmd)->parser(imreq->value.value, imreq->value.len);
      *rsp = (*cmd)->run(db, client, request);
      (*cmd)->freeer(request);
      break;
    }
  }
  if (!hasrun)
    ac_log(AC_LOG_ERROR, "protobuf decode failure: request type %u unknown",
           imreq->type);
  freeIMRequest(imreq);
  return 0;
}
