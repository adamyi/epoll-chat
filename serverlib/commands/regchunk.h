#ifndef IM_COMMAND_REGCHUNK_H_
#define IM_COMMAND_REGCHUNK_H_

#include "achelper/ac_protobuf.h"

#include "lib/client.h"
// do not sort
#include "lib/auth.h"
#include "lib/file.h"
#include "proto/IMResponse.pb.h"
#include "proto/RegisterChunkRequest.pb.h"
#include "serverlib/command.h"

struct IMResponse *cmd_regchunk_impl(UserDb *db, int epollfd,
                                     im_client_t *client,
                                     struct RegisterChunkRequest *req);

extern const im_command_t cmd_regchunk;

extern trie_file_t *fileDb;

#endif
