#ifndef IM_COMMAND_LOGIN_H_
#define IM_COMMAND_LOGIN_H_

#include "achelper/ac_protobuf.h"

#include "lib/client.h"
// do not sort
#include "proto/IMResponse.pb.h"
#include "proto/LoginRequest.pb.h"
#include "lib/auth.h"
#include "serverlib/command.h"

struct IMResponse *cmd_login_impl(UserDb *db, int epollfd, im_client_t *client,
                                  struct LoginRequest *req);

extern const im_command_t cmd_login;

#endif
