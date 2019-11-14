#define _GNU_SOURCE

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "achelper/ac_log.h"

#include "proto/IMRequest.pb.h"
#include "stopprivate.h"

#include "lib/client.h"

#include "lib/auth.h"
#include "lib/response.h"
#include "lib/socket.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"

struct IMRequest *cmd_stopprivate_impl(int epollfd, im_client_t *client,
                                       char *req) {
  char *username = req + 12;

  user_t *user = findUser(p2pdb, username);
  if (user == NULL || user->client == NULL) {
    ac_log(AC_LOG_ERROR, "Private messaging with %s has not been enabled",
           username);
    return NULL;
  }

  user->client->close_prehook = NULL;
  close_socket(epollfd, NULL, user->client);
  printf("Ended private messaging with %s\n", username);

  return NULL;
}

const im_command_t cmd_stopprivate = {.prefix = "stopprivate ",
                                      .run = cmd_stopprivate_impl};
#pragma GCC diagnostic pop
