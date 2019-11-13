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

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"

struct IMResponse *cmd_block_impl(UserDb *db, int epollfd, im_client_t *client,
                                  struct BlockRequest *req) {
  struct IMResponse *rsp = malloc(sizeof(struct IMResponse));
  rsp->success = false;
  if (!isUserLoggedIn(db, client->user)) {
    rsp->msg.len = asprintf(&(rsp->msg.value), "You are not logged in");
  } else {
    user_t *user = findUser(db, (char *)req->username.value);
    if (user == NULL) {
      rsp->msg.len = asprintf(&(rsp->msg.value), "%s is not present on server",
                              req->username.value);
    } else if (user == client->user) {
      rsp->msg.len = asprintf(&(rsp->msg.value), "cannot block yourself");
    } else if (hasBlockedUser(client->user, user)) {
      rsp->msg.len = asprintf(&(rsp->msg.value), "%s is already blocked",
                              req->username.value);
    } else {
      blockUser(client->user, user);
      rsp->success = true;
      rsp->msg.len =
          asprintf(&(rsp->msg.value), "%s is blocked", req->username.value);
    }
  }
  return rsp;
}

const im_command_t cmd_block = {.type = 4,
                                .run = cmd_block_impl,
                                .parser = parseBlockRequestFromBytes,
                                .freeer = freeBlockRequest};
#pragma GCC diagnostic pop
