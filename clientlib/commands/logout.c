#define _GNU_SOURCE

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "achelper/ac_log.h"

#include "logout.h"
#include "proto/IMRequest.pb.h"
#include "proto/LogOutRequest.pb.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"

#define LOGIN_SIZE_LIMIT 100
struct IMRequest *cmd_logout_impl(int epollfd, im_client_t *client, char *req) {
  struct IMRequest *ret = malloc(sizeof(struct IMRequest));
  ret->type = 6;
  struct LoginRequest *lreq = malloc(sizeof(struct LogOutRequest));
  ret->value.value = encodeLogOutRequestToBytes(lreq, &(ret->value.len));
  freeLogOutRequest(lreq);
  return ret;
}

const im_command_t cmd_logout = {.prefix = "logout", .run = cmd_logout_impl};
#pragma GCC diagnostic pop
