#ifndef IM_COMMAND_MESSAGE_H_
#define IM_COMMAND_MESSAGE_H_

#include "achelper/ac_protobuf.h"

#include "lib/client.h"
// do not sort
#include "proto/IMResponse.pb.h"
#include "proto/MessageRequest.pb.h"
#include "lib/auth.h"
#include "serverlib/command.h"

struct IMResponse *cmd_message_impl(UserDb *db, int epollfd,
                                    im_client_t *client,
                                    struct MessageRequest *req);

extern const im_command_t cmd_message;

#endif
