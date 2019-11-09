#define _GNU_SOURCE

#include <stdbool.h>
#include <stdlib.h>
#include "achelper/ac_log.h"

#include "message.h"
#include "proto/IMResponse.pb.h"
#include "proto/MessageRequest.pb.h"
#include "serverlib/auth.h"
#include "serverlib/socket.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"
bool sendMessage(UserDb *db, int epollfd, user_t *to, user_t *from, char *msg) {
  // TODO: block check
  struct IMResponse *rsp = malloc(sizeof(struct IMResponse));
  rsp->success = true;
  rsp->msg.len = asprintf(&(rsp->msg.value), "%s: %s", from->username, msg);
  send_response_to_user(db, epollfd, to, rsp);
  return true;
}

struct IMResponse *cmd_message_impl(UserDb *db, int epollfd,
                                    im_client_t *client,
                                    struct MessageRequest *req) {
  if (!isUserLoggedIn(db, client->user)) {
    struct IMResponse *rsp = malloc(sizeof(struct IMResponse));
    rsp->success = false;
    rsp->msg.len = asprintf(&(rsp->msg.value), "You are not logged in.");
    return rsp;
  }
  if (req->broadcast) {
    linked_user_t *curr = db->first;
    while (curr != NULL) {
      if (curr->user != NULL && curr->user != client->user) {
        sendMessage(db, epollfd, curr->user, client->user,
                    (char *)req->msg.value);
      }
      curr = curr->next;
    }
  } else {
    user_t *user = findUser(db, (char *)req->username.value);
    sendMessage(db, epollfd, user, client->user, (char *)req->msg.value);
  }
  return NULL;
}

const im_command_t cmd_message = {.type = 2,
                                  .run = cmd_message_impl,
                                  .parser = parseMessageRequestFromBytes,
                                  .freeer = freeMessageRequest};
#pragma GCC diagnostic pop
