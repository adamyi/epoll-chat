#ifndef IM_COMMAND_LOGIN_H_
#define IM_COMMAND_LOGIN_H_

#include "achelper/ac_protobuf.h"

#include "lib/client.h"
// do not sort
#include "clientlib/command.h"
#include "lib/auth.h"
#include "proto/IMResponse.pb.h"
#include "proto/LoginRequest.pb.h"

struct IMRequest *cmd_login_impl(int epollfd, im_client_t *client, char *req);

extern const im_command_t cmd_login;

extern uint32_t listen_port;
extern char *loggedInUserName;

#endif
