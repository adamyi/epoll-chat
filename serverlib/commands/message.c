#define _GNU_SOURCE

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "achelper/ac_log.h"

#include "message.h"
#include "proto/IMResponse.pb.h"
#include "proto/MessageRequest.pb.h"
#include "lib/auth.h"
#include "lib/socket.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"

#define MAX_MESSAGE_ERROR_LENGTH 1024

static struct IMResponse *sendError(char *errmsg) {
  struct IMResponse *rsp = malloc(sizeof(struct IMResponse));
  rsp->success = false;
  rsp->msg.value = (uint8_t *)errmsg;
  rsp->msg.len = strlen(errmsg);
  return rsp;
}

static bool sendMessage(UserDb *db, int epollfd, user_t *to, user_t *from,
                        char *msg) {
  if (hasBlockedUser(to, from)) return false;
  struct IMResponse *rsp = malloc(sizeof(struct IMResponse));
  rsp->success = true;
  rsp->msg.len = asprintf(&(rsp->msg.value), "%s: %s", from->username, msg);
  send_response_to_user(db, epollfd, to, rsp);
  freeIMResponse(rsp);
  return true;
}

struct IMResponse *cmd_message_impl(UserDb *db, int epollfd,
                                    im_client_t *client,
                                    struct MessageRequest *req) {
  char *errmsg = malloc(MAX_MESSAGE_ERROR_LENGTH);
  if (!isUserLoggedIn(db, client->user)) {
    strcpy(errmsg, "You are not logged in.");
    return sendError(errmsg);
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
      sprintf(errmsg,
              "Your message could not be delivered to some of the recipients.");
      return sendError(errmsg);
    }
  } else {
    user_t *user = findUser(db, (char *)req->username.value);
    if (user == NULL) {
      sprintf(errmsg, "%s does not exist on server.", req->username.value);
      return sendError(errmsg);
    }
    if (!sendMessage(db, epollfd, user, client->user, (char *)req->msg.value)) {
      sprintf(errmsg,
              "Your message couldn't be delivered because %s has blocked you",
              req->username.value);
      return sendError(errmsg);
    }
  }
  free(errmsg);
  return NULL;
}

const im_command_t cmd_message = {.type = 2,
                                  .run = cmd_message_impl,
                                  .parser = parseMessageRequestFromBytes,
                                  .freeer = freeMessageRequest};
#pragma GCC diagnostic pop
