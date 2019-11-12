#define _GNU_SOURCE

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "achelper/ac_log.h"

#include "block.h"
#include "proto/BlockRequest.pb.h"
#include "proto/IMRequest.pb.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"

struct IMRequest *cmd_block_impl(int epollfd, im_client_t *client, char *req) {
  struct IMRequest *ret = malloc(sizeof(struct IMRequest));
  ret->type = 3;
  char *username = req + 6;

  struct BlockRequest *mreq = malloc(sizeof(struct BlockRequest));
  mreq->username.len = asprintf(&(mreq->username.value), "%s", username);
  ret->value.value = encodeBlockRequestToBytes(mreq, &(ret->value.len));
  freeBlockRequest(mreq);
  return ret;
}

const im_command_t cmd_block = {
    .type = 3, .prefix = "block ", .run = cmd_block_impl};
#pragma GCC diagnostic pop
