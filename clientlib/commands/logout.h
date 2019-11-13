#ifndef IM_COMMAND_LOGOUT_H_
#define IM_COMMAND_LOGOUT_H_

#include "achelper/ac_protobuf.h"

#include "lib/client.h"
// do not sort
#include "clientlib/command.h"
#include "lib/auth.h"
#include "proto/IMResponse.pb.h"
#include "proto/LogOutRequest.pb.h"

struct IMRequest *cmd_logout_impl(int epollfd, im_client_t *client, char *req);

extern const im_command_t cmd_logout;

#endif
