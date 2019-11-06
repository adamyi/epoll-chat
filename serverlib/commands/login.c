#include "achelper/ac_log.h"

#include "login.h"
#include "proto/LoginRequest.pb.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"
void cmd_login_impl(UserDb *db, struct im_client *client,
                    struct LoginRequest *req) {
#pragma GCC diagnostic pop
  user_t *user = NULL;
  int res = login(db, (char *)req->username.value, (char *)req->password.value,
                  &user);
  switch (res) {
    case 0:  // success
      ac_log(AC_LOG_INFO, "login success");
      client->user = user;
      break;
    case 1:  // user not found
    case 3:  // wrong password
      ac_log(AC_LOG_INFO, "username or password is wrong");
      break;
    case 2:  // user blocked
      ac_log(AC_LOG_INFO, "user is blocked");
      break;
    case 4:  // another session in place
      ac_log(AC_LOG_INFO, "another session is in place");
      break;
  }
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"
const im_command_t cmd_login = {.type = 1,
                                .run = cmd_login_impl,
                                .parser = parseLoginRequestFromBytes,
                                .freeer = freeLoginRequest};
#pragma GCC diagnostic pop
