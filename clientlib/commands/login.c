#define _GNU_SOURCE

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "achelper/ac_log.h"

#include "login.h"
#include "proto/IMRequest.pb.h"
#include "proto/LoginRequest.pb.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"

#define LOGIN_SIZE_LIMIT 100
struct IMRequest *cmd_login_impl(int epollfd, im_client_t *client, char *req) {
  struct IMRequest *ret = malloc(sizeof(struct IMRequest));
  ret->type = 1;
  struct LoginRequest *lreq = malloc(sizeof(struct LoginRequest));
  char *username = malloc(LOGIN_SIZE_LIMIT);
  char *password = malloc(LOGIN_SIZE_LIMIT);
  lreq->username.value = (uint8_t *)username;
  lreq->password.value = (uint8_t *)password;
  printf("Username: ");
  fgets(username, LOGIN_SIZE_LIMIT, stdin);
  username[strlen(username) - 1] = '\0';
  printf("Password: ");
  fgets(password, LOGIN_SIZE_LIMIT, stdin);
  password[strlen(password) - 1] = '\0';
  lreq->username.len = strlen(username);
  lreq->password.len = strlen(password);
  loggedInUserName = strdup(username);
  lreq->port = listen_port;
  ret->value.value = encodeLoginRequestToBytes(lreq, &(ret->value.len));
  freeLoginRequest(lreq);
  return ret;
}

const im_command_t cmd_login = {.prefix = "login", .run = cmd_login_impl};
#pragma GCC diagnostic pop
