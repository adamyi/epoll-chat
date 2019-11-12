#ifndef IM_COMMAND_UNBLOCK_H_
#define IM_COMMAND_UNBLOCK_H_

#include "achelper/ac_protobuf.h"

#include "serverlib/client.h"
// do not sort
#include "proto/IMResponse.pb.h"
#include "proto/UnBlockRequest.pb.h"
#include "serverlib/auth.h"
#include "serverlib/command.h"

struct IMResponse *cmd_unblock_impl(UserDb *db, int epollfd,
                                    im_client_t *client,
                                    struct UnBlockRequest *req);

extern const im_command_t cmd_unblock;

#endif
