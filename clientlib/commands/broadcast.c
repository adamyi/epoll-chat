#define _GNU_SOURCE

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "achelper/ac_log.h"

#include "broadcast.h"
#include "proto/IMRequest.pb.h"
#include "proto/MessageRequest.pb.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"

struct IMRequest *cmd_broadcast_impl(int epollfd, im_client_t *client,
                                     char *req) {
  struct IMRequest *ret = malloc(sizeof(struct IMRequest));
  ret->type = 2;
  char *message = req + 10;

  struct MessageRequest *mreq = malloc(sizeof(struct MessageRequest));
  mreq->username.len = 0;
  mreq->msg.len = asprintf(&(mreq->msg.value), "%s", message);
  mreq->broadcast = true;
  ret->value.value = encodeMessageRequestToBytes(mreq, &(ret->value.len));
  return ret;
}

const im_command_t cmd_broadcast = {
    .type = 2, .prefix = "broadcast ", .run = cmd_broadcast_impl};
#pragma GCC diagnostic pop
