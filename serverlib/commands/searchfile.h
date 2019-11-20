#ifndef IM_COMMAND_SEARCHFILE_H_
#define IM_COMMAND_SEARCHFILE_H_

#include "achelper/ac_protobuf.h"

#include "lib/client.h"
// do not sort
#include "lib/auth.h"
#include "lib/file.h"
#include "proto/IMResponse.pb.h"
#include "proto/SearchFileRequest.pb.h"
#include "serverlib/command.h"

struct IMResponse *cmd_searchfile_impl(UserDb *db, int epollfd,
                                       im_client_t *client,
                                       struct SearchFileRequest *req);

extern const im_command_t cmd_searchfile;

extern trie_file_t *fileDb;

#endif
