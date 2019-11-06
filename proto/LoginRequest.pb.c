// clang-format off
// this file is auto-generated with ac_protoc. Do not modify by hand.

#include <memory.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "achelper/ac_protobuf.h"

#include "LoginRequest.pb.h"

struct LoginRequest *parseLoginRequestFromProtobufMsg(ac_protobuf_message_t *msg) {
  struct LoginRequest *ret = malloc(sizeof(struct LoginRequest));
  ac_protobuf_field_t *username_f = ac_find_protobuf_field_in_msg(msg, 1);
  ret->username.value = malloc(username_f->len);
  memcpy(ret->username.value, username_f->value, username_f->len);
  ret->username.len = username_f->len;
  ac_protobuf_field_t *password_f = ac_find_protobuf_field_in_msg(msg, 2);
  ret->password.value = malloc(password_f->len);
  memcpy(ret->password.value, password_f->value, password_f->len);
  ret->password.len = password_f->len;
  return ret;
}

struct LoginRequest *parseLoginRequestFromBytes(uint8_t *bytes, size_t len) {
  ac_protobuf_message_t *msg = ac_decode_protobuf_msg(bytes, len);
  struct LoginRequest *ret = parseLoginRequestFromProtobufMsg(msg);
  ac_protobuf_free_msg(msg);
  return ret;
}

void freeLoginRequest(struct LoginRequest *val) {
  free(val->username.value);
  free(val->password.value);
  free(val);
}
// clang-format on
