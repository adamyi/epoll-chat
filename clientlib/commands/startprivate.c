#define _GNU_SOURCE

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "achelper/ac_log.h"

#include "proto/GetIPRequest.pb.h"
#include "proto/IMRequest.pb.h"
#include "startprivate.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"

struct IMRequest *cmd_startprivate_impl(int epollfd, im_client_t *client,
                                        char *req) {
  struct IMRequest *ret = malloc(sizeof(struct IMRequest));
  ret->type = 7;
  char *username = req + 13;

  struct GetIPRequest *mreq = malloc(sizeof(struct GetIPRequest));
  mreq->username.len = asprintf(&(mreq->username.value), "%s", username);
  ret->value.value = encodeGetIPRequestToBytes(mreq, &(ret->value.len));
  freeGetIPRequest(mreq);
  return ret;
}

const im_command_t cmd_startprivate = {.prefix = "startprivate ",
                                       .run = cmd_startprivate_impl};
#pragma GCC diagnostic pop
