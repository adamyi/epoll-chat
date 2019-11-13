#define _GNU_SOURCE

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "achelper/ac_log.h"

#include "lib/client.h"

#include "block.h"
#include "lib/auth.h"
#include "lib/socket.h"
#include "proto/BlockRequest.pb.h"
#include "proto/IMResponse.pb.h"
#include "proto/TextResponse.pb.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"

struct IMResponse *cmd_block_impl(UserDb *db, int epollfd, im_client_t *client,
                                  struct BlockRequest *req) {
  struct TextResponse *irsp = malloc(sizeof(struct IMResponse));
  bool success = false;
  if (!isUserLoggedIn(db, client->user)) {
    irsp->msg.len = asprintf(&(irsp->msg.value), "You are not logged in");
  } else {
    user_t *user = findUser(db, (char *)req->username.value);
    if (user == NULL) {
      irsp->msg.len =
          asprintf(&(irsp->msg.value), "%s is not present on server",
                   req->username.value);
    } else if (user == client->user) {
      irsp->msg.len = asprintf(&(irsp->msg.value), "cannot block yourself");
    } else if (hasBlockedUser(client->user, user)) {
      irsp->msg.len = asprintf(&(irsp->msg.value), "%s is already blocked",
                               req->username.value);
    } else {
      blockUser(client->user, user);
      success = true;
      irsp->msg.len =
          asprintf(&(irsp->msg.value), "%s is blocked", req->username.value);
    }
  }
  return encodeTextResponseToIMResponseAndFree(irsp, success);
}

const im_command_t cmd_block = {.type = 4,
                                .run = cmd_block_impl,
                                .parser = parseBlockRequestFromBytes,
                                .freeer = freeBlockRequest};
#pragma GCC diagnostic pop
