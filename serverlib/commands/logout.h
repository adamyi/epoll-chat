#ifndef IM_COMMAND_LOGOUT_H_
#define IM_COMMAND_LOGOUT_H_

#include "achelper/ac_protobuf.h"

#include "lib/client.h"
// do not sort
#include "lib/auth.h"
#include "proto/IMResponse.pb.h"
#include "proto/LogOutRequest.pb.h"
#include "serverlib/command.h"

struct IMResponse *cmd_logout_impl(UserDb *db, int epollfd, im_client_t *client,
                                   struct LogOutRequest *req);

extern const im_command_t cmd_logout;

#endif
