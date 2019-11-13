#ifndef IM_COMMAND_WHOELSE_H_
#define IM_COMMAND_WHOELSE_H_

#include "achelper/ac_protobuf.h"

#include "lib/client.h"
// do not sort
#include "lib/auth.h"
#include "proto/IMResponse.pb.h"
#include "proto/WhoElseRequest.pb.h"
#include "serverlib/command.h"

struct IMResponse *cmd_whoelse_impl(UserDb *db, int epollfd,
                                    im_client_t *client,
                                    struct WhoElseRequest *req);

extern const im_command_t cmd_whoelse;

#endif
