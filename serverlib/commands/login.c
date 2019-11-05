#include "achelper/ac_log.h"

#include "login.h"

void cmd_login_impl(UserDb *db, struct im_client *client,
                    ac_protobuf_message_t *req) {
  ac_protobuf_field_t *username = ac_find_protobuf_field_in_msg(req, 1);
  if (username == NULL) {
    ac_log(AC_LOG_ERROR, "username does not exist");
  }
  ac_protobuf_field_t *password = ac_find_protobuf_field_in_msg(req, 2);
  if (password == NULL) {
    ac_log(AC_LOG_ERROR, "password does not exist");
  }
  int res = login(db, (char *)(username->value), (char *)(password->value));
  switch (res) {
    case 0:  // success
      ac_log(AC_LOG_INFO, "login success");
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

const im_command_t cmd_login = {.type = 1, .run = cmd_login_impl};
