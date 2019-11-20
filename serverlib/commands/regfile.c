#define _GNU_SOURCE

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "achelper/ac_log.h"

#include "lib/client.h"

#include "lib/auth.h"
#include "lib/file.h"
#include "lib/socket.h"
#include "proto/IMResponse.pb.h"
#include "proto/RegisterFileRequest.pb.h"
#include "proto/TextResponse.pb.h"
#include "regfile.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"

struct IMResponse *cmd_regfile_impl(UserDb *db, int epollfd,
                                    im_client_t *client,
                                    struct RegisterFileRequest *req) {
  struct TextResponse *irsp = malloc(sizeof(struct IMResponse));
  bool success = false;
  if (!isUserLoggedIn(db, client->user)) {
    irsp->msg.len = asprintf(&(irsp->msg.value), "You are not logged in");
  } else {
    trie_file_t *f = addFile(fileDb, (char *)req->filename.value, req->chunks);
    if (f == NULL) {
      irsp->msg.len = asprintf(&(irsp->msg.value), "%s already exists",
                               req->filename.value);
    } else {
      regAllFileChunk(client, f);
      success = true;
      irsp->msg.len = asprintf(&(irsp->msg.value), "%s successfully registered",
                               req->filename.value);
    }
  }
  return encodeTextResponseToIMResponseAndFree(irsp, success);
}

const im_command_t cmd_regfile = {.type = 8,
                                  .run = cmd_regfile_impl,
                                  .parser = parseRegisterFileRequestFromBytes,
                                  .freeer = freeRegisterFileRequest};
#pragma GCC diagnostic pop
