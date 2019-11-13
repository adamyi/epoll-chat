#ifndef IM_COMMAND_BLOCK_H_
#define IM_COMMAND_BLOCK_H_

#include "achelper/ac_protobuf.h"

#include "lib/client.h"
// do not sort
#include "clientlib/command.h"
#include "lib/auth.h"
#include "proto/BlockRequest.pb.h"
#include "proto/IMResponse.pb.h"

struct IMRequest *cmd_block_impl(int epollfd, im_client_t *client, char *req);

extern const im_command_t cmd_block;

#endif
