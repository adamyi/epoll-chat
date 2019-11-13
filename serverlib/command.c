#include <time.h>

#include "achelper/ac_log.h"
#include "achelper/ac_protobuf.h"

#include "lib/client.h"
// do not sort
#include "lib/auth.h"

#include "proto/IMRequest.pb.h"
#include "proto/IMResponse.pb.h"

#include "serverlib/commands/block.h"
#include "serverlib/commands/login.h"
#include "serverlib/commands/logout.h"
#include "serverlib/commands/message.h"
#include "serverlib/commands/unblock.h"
#include "serverlib/commands/whoelse.h"

const im_command_t *enabled_commands[] = {
    &cmd_login,  &cmd_message, &cmd_block, &cmd_unblock,
    &cmd_logout, &cmd_whoelse, NULL};

size_t parse_command(UserDb *db, int epollfd, im_client_t *client, uint8_t *cmd,
                     size_t len, struct IMResponse **rsp) {
  size_t ret = 0;
  ac_protobuf_message_t *msg =
      ac_decode_protobuf_msg_with_n_fields(cmd, len, 2, &ret);
  if (msg == NULL) {
    ac_log(AC_LOG_ERROR, "protobuf decode failure: invalid protobuf");
    return 0;
  }
  struct IMRequest *imreq = parseIMRequestFromProtobufMsg(msg);

  ac_protobuf_print_msg(msg);
  bool hasrun = false;
  for (const im_command_t **cmd = enabled_commands; *cmd != NULL; cmd++) {
    if ((*cmd)->type == imreq->type) {
      ac_log(AC_LOG_DEBUG, "found implementation for request type %u",
             imreq->type);
      hasrun = true;
      size_t tmp = 0;
      void *request =
          (*cmd)->parser(imreq->value.value, imreq->value.len, &tmp);
      *rsp = (*cmd)->run(db, epollfd, client, request);
      (*cmd)->freeer(request);
      if (isUserLoggedIn(db, client->user))
        client->user->last_active = time(NULL);
      break;
    }
  }
  if (!hasrun)
    ac_log(AC_LOG_ERROR, "protobuf decode failure: request type %u unknown",
           imreq->type);
  freeIMRequest(imreq);
  return ret;
}
