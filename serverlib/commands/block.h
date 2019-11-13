#ifndef IM_COMMAND_BLOCK_H_
#define IM_COMMAND_BLOCK_H_

#include "achelper/ac_protobuf.h"

#include "lib/client.h"
// do not sort
#include "lib/auth.h"
#include "proto/BlockRequest.pb.h"
#include "proto/IMResponse.pb.h"
#include "serverlib/command.h"

struct IMResponse *cmd_block_impl(UserDb *db, int epollfd, im_client_t *client,
                                  struct BlockRequest *req);

extern const im_command_t cmd_block;

#endif
