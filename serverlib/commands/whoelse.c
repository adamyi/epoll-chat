#define _GNU_SOURCE

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "achelper/ac_log.h"

#include "lib/client.h"

#include "lib/auth.h"
#include "lib/socket.h"
#include "proto/IMResponse.pb.h"
#include "proto/WhoElseRequest.pb.h"
#include "whoelse.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"

struct IMResponse *cmd_whoelse_impl(UserDb *db, int epollfd,
                                    im_client_t *client,
                                    struct WhoElseRequest *req) {
  char *rsp;
  if (!isUserLoggedIn(db, client->user))
    return encodeTextToIMResponse("You are not logged in", false);
  linked_user_t *userHistory;
  if (req->limitTime)
    userHistory = loggedInUsersInPastSeconds(
        db, req->pastSeconds, req->currentlyOnline, client->user);
  else
    userHistory = loggedInUsers(db, req->currentlyOnline, client->user);
  if (userHistory == NULL)
    return encodeTextToIMResponse("No other users other than you", false);
  userLinkedListToString(userHistory, &rsp);
  freeUserLinkedList(userHistory, false);

  struct IMResponse *ret = encodeTextToIMResponse(rsp, true);
  free(rsp);
  return ret;
}

const im_command_t cmd_whoelse = {.type = 3,
                                  .run = cmd_whoelse_impl,
                                  .parser = parseWhoElseRequestFromBytes,
                                  .freeer = freeWhoElseRequest};
#pragma GCC diagnostic pop
