#define _GNU_SOURCE

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "achelper/ac_log.h"

#include "lib/client.h"

#include "getip.h"
#include "lib/auth.h"
#include "lib/socket.h"
#include "proto/GetIPRequest.pb.h"
#include "proto/GetIPResponse.pb.h"
#include "proto/IMResponse.pb.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"

struct IMResponse *cmd_getip_impl(UserDb *db, int epollfd, im_client_t *client,
                                  struct GetIPRequest *req) {
  if (!isUserLoggedIn(db, client->user))
    return encodeTextToIMResponse("You are not logged in", false);
  user_t *user = findUser(db, (char *)req->username.value);
  if (user == NULL)
    return encodeTextToIMResponse("User is not present on server", false);
  if (user == client->user)
    return encodeTextToIMResponse("Cannot startprivate with yourself", false);
  if (hasBlockedUser(user, client->user))
    return encodeTextToIMResponse("Target has blocked you", false);
  if (!isUserLoggedIn(db, user))
    return encodeTextToIMResponse("Target is not online", false);
  struct GetIPResponse *irsp = malloc(sizeof(struct GetIPResponse));
  irsp->username.len =
      asprintf(&(irsp->username.value), "%s", req->username.value);
  irsp->addr.len = sizeof(struct sockaddr_in);
  irsp->addr.value = malloc(sizeof(struct sockaddr_in));
  memcpy(irsp->addr.value, &(user->client->addr), sizeof(struct sockaddr_in));

  struct IMResponse *ret = malloc(sizeof(struct IMResponse));
  ret->type = 3;
  ret->value.value = encodeGetIPResponseToBytes(irsp, &(ret->value.len));
  freeGetIPResponse(irsp);
  ret->success = true;
  return ret;
}

const im_command_t cmd_getip = {.type = 7,
                                .run = cmd_getip_impl,
                                .parser = parseGetIPRequestFromBytes,
                                .freeer = freeGetIPRequest};
#pragma GCC diagnostic pop
