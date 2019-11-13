#define _GNU_SOURCE

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "achelper/ac_log.h"

#include "lib/client.h"

#include "lib/auth.h"
#include "lib/socket.h"
#include "message.h"
#include "proto/IMResponse.pb.h"
#include "proto/TextResponse.pb.h"
#include "proto/MessageRequest.pb.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"

#define MAX_MESSAGE_ERROR_LENGTH 1024

static bool sendMessage(UserDb *db, int epollfd, user_t *to, user_t *from,
                        char *msg) {
  if (hasBlockedUser(to, from)) return false;
  struct TextResponse *rsp = malloc(sizeof(struct TextResponse));
  rsp->msg.len = asprintf(&(rsp->msg.value), "%s: %s", from->username, msg);
  send_response_to_user(db, epollfd, to, encodeTextResponseToIMResponseAndFree(rsp, true));
  return true;
}

struct IMResponse *cmd_message_impl(UserDb *db, int epollfd,
                                    im_client_t *client,
                                    struct MessageRequest *req) {
  if (!isUserLoggedIn(db, client->user)) {
    return encodeTextToIMResponse("You are not logged in", false);
  }
  if (req->broadcast) {
    linked_user_t *curr = db->first;
    bool success = true;
    while (curr != NULL) {
      if (curr->user != NULL && curr->user != client->user) {
        success &= sendMessage(db, epollfd, curr->user, client->user,
                               (char *)req->msg.value);
      }
      curr = curr->next;
    }
    if (!success) {
      return encodeTextToIMResponse("Your message could not be delivered to some of the recipients.", false);
    }
  } else {
    user_t *user = findUser(db, (char *)req->username.value);
    if (user == NULL) {
      return encodeTextToIMResponse("Recipient user does not exist on server.", false);
    }
    if (!sendMessage(db, epollfd, user, client->user, (char *)req->msg.value)) {
      return encodeTextToIMResponse("Your message couldn't be delivered because the recipient has blocked you", false);
    }
  }
  return NULL;
}

const im_command_t cmd_message = {.type = 2,
                                  .run = cmd_message_impl,
                                  .parser = parseMessageRequestFromBytes,
                                  .freeer = freeMessageRequest};
#pragma GCC diagnostic pop
