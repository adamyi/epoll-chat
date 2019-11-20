#ifndef IM_COMMAND_REGFILE_H_
#define IM_COMMAND_REGFILE_H_

#include "achelper/ac_protobuf.h"

#include "lib/client.h"
// do not sort
#include "lib/auth.h"
#include "lib/file.h"
#include "proto/IMResponse.pb.h"
#include "proto/RegisterFileRequest.pb.h"
#include "serverlib/command.h"

struct IMResponse *cmd_regfile_impl(UserDb *db, int epollfd,
                                    im_client_t *client,
                                    struct RegisterFileRequest *req);

extern const im_command_t cmd_regfile;

extern trie_file_t *fileDb;

#endif
