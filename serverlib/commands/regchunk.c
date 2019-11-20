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
#include "regchunk.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"

struct IMResponse *cmd_regchunk_impl(UserDb *db, int epollfd,
                                     im_client_t *client,
                                     struct RegisterChunkRequest *req) {
  struct TextResponse *irsp = malloc(sizeof(struct IMResponse));
  bool success = false;
  if (!isUserLoggedIn(db, client->user)) {
    irsp->msg.len = asprintf(&(irsp->msg.value), "You are not logged in");
  } else {
    trie_file_t *f = findFile(fileDb, (char *)req->filename.value);
    if (f == NULL) {
      irsp->msg.len = asprintf(&(irsp->msg.value),
                               "%s does not exist on server. Register it first",
                               req->filename.value);
    } else {
      size_t n = 0;
      uint32_t **chunks = parse_packed_uint32(&(req->chunks), &n);
      regFileChunk(client, f, chunks);
      free_parsed_packed_uint32(chunks);

      success = true;
      irsp->msg.len = asprintf(&(irsp->msg.value),
                               "Successfully registered %zu chunks for %s", n,
                               req->filename.value);
    }
  }
  return encodeTextResponseToIMResponseAndFree(irsp, success);
}

const im_command_t cmd_regchunk = {.type = 9,
                                   .run = cmd_regchunk_impl,
                                   .parser = parseRegisterChunkRequestFromBytes,
                                   .freeer = freeRegisterChunkRequest};
#pragma GCC diagnostic pop
