#define _GNU_SOURCE

#include <stdbool.h>
#include <stdlib.h>
#include "achelper/ac_log.h"

#include "lib/socket.h"
#include "logout.h"
#include "proto/IMResponse.pb.h"
#include "proto/LogOutRequest.pb.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"
struct IMResponse *cmd_logout_impl(UserDb *db, int epollfd, im_client_t *client,
                                   struct LogOutRequest *req) {
  if (!isUserLoggedIn(db, client->user)) {
    return encodeTextToIMResponse("You are not logged in", false);
  }
  close_socket(epollfd, db, client);
  return NULL;
}

const im_command_t cmd_logout = {.type = 6,
                                 .run = cmd_logout_impl,
                                 .parser = parseLogOutRequestFromBytes,
                                 .freeer = freeLogOutRequest};
#pragma GCC diagnostic pop
