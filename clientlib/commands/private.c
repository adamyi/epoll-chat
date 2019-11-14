#define _GNU_SOURCE

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "achelper/ac_log.h"

#include "private.h"
#include "proto/IMRequest.pb.h"

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
  im_client_t *uclient = user->client;
  if (user == NULL || uclient == NULL) {
    ac_log(AC_LOG_ERROR, "Private messaging with %s has not been enabled",
           username);
    return NULL;
  }

  char *msg;
  asprintf(&msg, "%s (private): %s", loggedInUserName, message);
  ac_log(AC_LOG_DEBUG, "sending: %s", msg);
  struct IMResponse *rsp = encodeTextToIMResponse(msg, true);
  send_response_to_client(epollfd, user->client, rsp);
  free(msg);
  freeIMResponse(rsp);

  return NULL;
}

const im_command_t cmd_private = {.prefix = "private ",
                                  .run = cmd_private_impl};
#pragma GCC diagnostic pop
