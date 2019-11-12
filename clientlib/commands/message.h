#ifndef IM_COMMAND_MESSAGE_H_
#define IM_COMMAND_MESSAGE_H_

#include "achelper/ac_protobuf.h"

#include "lib/client.h"
// do not sort
#include "lib/auth.h"
#include "clientlib/command.h"
#include "proto/IMResponse.pb.h"
#include "proto/MessageRequest.pb.h"

struct IMRequest *cmd_message_impl(int epollfd, im_client_t *client, char *req);

extern const im_command_t cmd_message;

#endif
