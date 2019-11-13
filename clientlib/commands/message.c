#define _GNU_SOURCE

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "achelper/ac_log.h"

#include "message.h"
#include "proto/IMRequest.pb.h"
#include "proto/MessageRequest.pb.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"

struct IMRequest *cmd_message_impl(int epollfd, im_client_t *client,
                                   char *req) {
  struct IMRequest *ret = malloc(sizeof(struct IMRequest));
  ret->type = 2;
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

  struct MessageRequest *mreq = malloc(sizeof(struct MessageRequest));
  mreq->username.len = asprintf(&(mreq->username.value), "%s", username);
  mreq->msg.len = asprintf(&(mreq->msg.value), "%s", message);
  mreq->broadcast = false;
  ret->value.value = encodeMessageRequestToBytes(mreq, &(ret->value.len));
  freeMessageRequest(mreq);
  return ret;
}

const im_command_t cmd_message = {.prefix = "message ",
                                  .run = cmd_message_impl};
#pragma GCC diagnostic pop
