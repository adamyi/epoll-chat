#ifndef IM_COMMAND_LOGIN_H_
#define IM_COMMAND_LOGIN_H_

#include "achelper/ac_protobuf.h"

#include "serverlib/client.h"
// do not sort
#include "proto/LoginRequest.pb.h"
#include "serverlib/auth.h"
#include "serverlib/command.h"

void cmd_login_impl(UserDb *db, struct im_client *client,
                    struct LoginRequest *req);

extern const im_command_t cmd_login;

#endif
