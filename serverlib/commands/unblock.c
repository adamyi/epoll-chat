#define _GNU_SOURCE

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "achelper/ac_log.h"

#include "serverlib/client.h"

#include "proto/IMResponse.pb.h"
#include "proto/UnBlockRequest.pb.h"
#include "serverlib/auth.h"
#include "serverlib/socket.h"
#include "unblock.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"

struct IMResponse *cmd_unblock_impl(UserDb *db, int epollfd,
                                    im_client_t *client,
                                    struct UnBlockRequest *req) {
  struct IMResponse *rsp = malloc(sizeof(struct IMResponse));
  if (!isUserLoggedIn(db, client->user)) {
    rsp->success = false;
    rsp->msg.len = asprintf(&(rsp->msg.value), "You are not logged in",
                            req->username.value);
  } else {
    user_t *user = findUser(db, (char *)req->username.value);
    if (user == NULL) {
      rsp->success = false;
      rsp->msg.len = asprintf(&(rsp->msg.value), "%s is not present on server",
                              req->username.value);
    } else if (user == client->user) {
      rsp->success = false;
      rsp->msg.len = asprintf(&(rsp->msg.value), "cannot unblock yourself");
    } else if (!hasBlockedUser(client->user, user)) {
      rsp->success = false;
      rsp->msg.len =
          asprintf(&(rsp->msg.value), "%s is not blocked", req->username.value);
    } else {
      unblockUser(client->user, user);
      rsp->success = true;
      rsp->msg.len =
          asprintf(&(rsp->msg.value), "%s is unblocked", req->username.value);
    }
  }
  return rsp;
}

const im_command_t cmd_unblock = {.type = 4,
                                  .run = cmd_unblock_impl,
                                  .parser = parseUnBlockRequestFromBytes,
                                  .freeer = freeUnBlockRequest};
#pragma GCC diagnostic pop
