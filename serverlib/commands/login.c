#define _GNU_SOURCE

#include <arpa/inet.h>
#include <stdbool.h>
#include <stdlib.h>
#include "achelper/ac_log.h"

#include "login.h"
#include "proto/IMResponse.pb.h"
#include "proto/LoginRequest.pb.h"
#include "proto/TextResponse.pb.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"
struct IMResponse *cmd_login_impl(UserDb *db, int epollfd, im_client_t *client,
                                  struct LoginRequest *req) {
  user_t *user = NULL;
  int res = login(db, epollfd, (char *)(req->username.value),
                  (char *)(req->password.value), &user);
  switch (res) {
    case 0:  // success
      ac_log(AC_LOG_INFO, "login success");
      client->user = user;
      user->client = client;
      client->addr.sin_port = htons(req->port);
      struct TextResponse *irsp = malloc(sizeof(struct TextResponse));
      irsp->msg.len =
          asprintf(&(irsp->msg.value), "Welcome %s!", user->username);
      return encodeTextResponseToIMResponseAndFree(irsp, true);
    case 1:  // user not found
      return encodeExitTextToIMResponse("user does not exist", false);
    case 3:  // wrong password
      return encodeTextToIMResponse("password is wrong", false);
    case 2:  // user blocked
      return encodeExitTextToIMResponse("you are blocked! try again later",
                                        false);
    case 4:  // another session in place
      return encodeExitTextToIMResponse("another session is in place", false);
  }
  ac_log(AC_LOG_ERROR, "unrecognized status code for login");
  return NULL;
}

const im_command_t cmd_login = {.type = 1,
                                .run = cmd_login_impl,
                                .parser = parseLoginRequestFromBytes,
                                .freeer = freeLoginRequest};
#pragma GCC diagnostic pop
