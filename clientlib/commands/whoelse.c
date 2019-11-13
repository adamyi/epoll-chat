#define _GNU_SOURCE

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "achelper/ac_log.h"

#include "proto/IMRequest.pb.h"
#include "proto/WhoElseRequest.pb.h"
#include "whoelse.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"

#define LOGIN_SIZE_LIMIT 100
struct IMRequest *cmd_whoelse_impl(int epollfd, im_client_t *client,
                                   char *req) {
  struct IMRequest *ret = malloc(sizeof(struct IMRequest));
  ret->type = 3;
  struct WhoElseRequest *lreq = malloc(sizeof(struct WhoElseRequest));
  lreq->limitTime = false;
  lreq->pastSeconds = 0;
  lreq->currentlyOnline = true;
  ret->value.value = encodeWhoElseRequestToBytes(lreq, &(ret->value.len));
  freeWhoElseRequest(lreq);
  return ret;
}

const im_command_t cmd_whoelse = {.prefix = "whoelse", .run = cmd_whoelse_impl};
#pragma GCC diagnostic pop
