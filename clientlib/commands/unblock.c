#define _GNU_SOURCE

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "achelper/ac_log.h"

#include "proto/IMRequest.pb.h"
#include "proto/UnBlockRequest.pb.h"
#include "unblock.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"

struct IMRequest *cmd_unblock_impl(int epollfd, im_client_t *client,
                                   char *req) {
  struct IMRequest *ret = malloc(sizeof(struct IMRequest));
  ret->type = 4;
  char *username = req + 8;

  struct UnBlockRequest *mreq = malloc(sizeof(struct UnBlockRequest));
  mreq->username.len = asprintf(&(mreq->username.value), "%s", username);
  ret->value.value = encodeUnBlockRequestToBytes(mreq, &(ret->value.len));
  freeUnBlockRequest(mreq);
  return ret;
}

const im_command_t cmd_unblock = {
    .type = 4, .prefix = "unblock ", .run = cmd_unblock_impl};
#pragma GCC diagnostic pop
