#define _GNU_SOURCE

#include <stdbool.h>
#include <stdlib.h>
#include "achelper/ac_log.h"

#include "login.h"
#include "proto/IMResponse.pb.h"
#include "proto/LoginRequest.pb.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"
struct IMResponse *cmd_login_impl(UserDb *db, int epollfd, im_client_t *client,
                                  struct LoginRequest *req) {
  struct IMResponse *rsp = malloc(sizeof(struct IMResponse));
  rsp->success = false;
  user_t *user = NULL;
  int res = login(db, (char *)req->username.value, (char *)req->password.value,
                  &user);
  switch (res) {
    case 0:  // success
      ac_log(AC_LOG_INFO, "login success");
      client->user = user;
      user->client = client;
      rsp->success = true;
      rsp->msg.len = asprintf(&(rsp->msg.value), "Welcome %s!", user->username);
      break;
    case 1:  // user not found
    case 3:  // wrong password
      ac_log(AC_LOG_INFO, "username or password is wrong");
      rsp->msg.len =
          asprintf(&(rsp->msg.value), "username or password is wrong");
      break;
    case 2:  // user blocked
      ac_log(AC_LOG_INFO, "user is blocked");
      rsp->msg.len =
          asprintf(&(rsp->msg.value), "you are blocked! try again later");
      break;
    case 4:  // another session in place
      ac_log(AC_LOG_INFO, "another session is in place");
      rsp->msg.len = asprintf(&(rsp->msg.value), "another session is in place");
      break;
  }
  return rsp;
}

const im_command_t cmd_login = {.type = 1,
                                .run = cmd_login_impl,
                                .parser = parseLoginRequestFromBytes,
                                .freeer = freeLoginRequest};
#pragma GCC diagnostic pop
