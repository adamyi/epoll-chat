#define _GNU_SOURCE

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "achelper/ac_log.h"

#include "private.h"
#include "proto/ChunkDataRequest.pb.h"
#include "proto/IMRequest.pb.h"
#include "proto/IMResponse.pb.h"

#include "lib/client.h"

#include "lib/auth.h"
#include "lib/response.h"
#include "lib/socket.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"

struct IMRequest *cmd_private_impl(int epollfd, im_client_t *client,
                                   char *req) {
  char *username = req + 8;
  char *message = username;
  while (1) {
    if (*message == ' ') {
      *message = '\0';
      message++;
      break;
    }
    if (*message == '\0') {
      ac_log(AC_LOG_ERROR, "incomplete command");
      return NULL;
    }
    message++;
  }

  user_t *user = findUser(p2pdb, username);
  if (user == NULL || user->client == NULL) {
    ac_log(AC_LOG_ERROR,
           "Private messaging with %s has not been enabled. You need to use "
           "startprivate first",
           username);
    return NULL;
  }

  struct IMResponse *rsp;

  if (strncmp(message, "download ", 9) == 0) {
    char filename[BUFSIZ];
    uint32_t chunk;
    message += 9;
    if (sscanf(message, "%s %u", filename, &chunk) != 2) {
      ac_log(AC_LOG_ERROR,
             "Invalid syntax for downloading p2p file. Correct syntax: private "
             "<username> download <filename> <chunkid>");
      return NULL;
    }
    struct ChunkDataRequest *cdr = malloc(sizeof(struct ChunkDataRequest));
    cdr->filename.len = asprintf(&(cdr->filename.value), "%s", filename);
    cdr->chunk = chunk;
    rsp = malloc(sizeof(struct IMResponse));
    rsp->type = 5;
    rsp->success = true;
    rsp->value.value = encodeChunkDataRequestToBytes(cdr, &(rsp->value.len));
    freeChunkDataRequest(cdr);
  } else {
    char *msg;
    asprintf(&msg, "%s (private): %s", loggedInUserName, message);
    ac_log(AC_LOG_DEBUG, "sending: %s", msg);
    rsp = encodeTextToIMResponse(msg, true);
    free(msg);
  }
  send_response_to_client(epollfd, user->client, rsp);
  freeIMResponse(rsp);

  return NULL;
}

const im_command_t cmd_private = {.prefix = "private ",
                                  .run = cmd_private_impl};
#pragma GCC diagnostic pop
