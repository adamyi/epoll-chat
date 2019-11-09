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
  ret->username.value = malloc(username_f->len + 1);
  memcpy(ret->username.value, username_f->value, username_f->len);
  ret->username.value[username_f->len] = 0;
  ret->username.len = username_f->len;
  ac_protobuf_field_t *password_f = ac_find_protobuf_field_in_msg(msg, 2);
  ret->password.value = malloc(password_f->len + 1);
  memcpy(ret->password.value, password_f->value, password_f->len);
  ret->password.value[password_f->len] = 0;
  ret->password.len = password_f->len;
  return ret;
}

struct LoginRequest *parseLoginRequestFromBytes(uint8_t *bytes, size_t len, size_t *read) {
  ac_protobuf_message_t *msg = ac_decode_protobuf_msg(bytes, len, read);
  struct LoginRequest *ret = parseLoginRequestFromProtobufMsg(msg);
  ac_protobuf_free_msg(msg);
  return ret;
}

ac_protobuf_message_t *encodeLoginRequestToProtobufMsg(struct LoginRequest *msg) {
  ac_protobuf_message_t *ret = malloc(sizeof(ac_protobuf_message_t));
  ac_protobuf_field_t *username_f = malloc(sizeof(ac_protobuf_field_t));
  username_f->id = 1;
  ret->fields = username_f;
  username_f->wiretype = 2;
  username_f->len = msg->username.len;
  username_f->value = malloc(username_f->len + 1);
  memcpy(username_f->value, msg->username.value, username_f->len);
  *(uint8_t *)(username_f->value + username_f->len) = 0;
  ac_protobuf_field_t *password_f = malloc(sizeof(ac_protobuf_field_t));
  password_f->id = 2;
  username_f->next = password_f;
  password_f->next = NULL;
  password_f->wiretype = 2;
  password_f->len = msg->password.len;
  password_f->value = malloc(password_f->len + 1);
  memcpy(password_f->value, msg->password.value, password_f->len);
  *(uint8_t *)(password_f->value + password_f->len) = 0;
  return ret;
}

uint8_t *encodeLoginRequestToBytes(struct LoginRequest *msg, size_t *len) {
  ac_protobuf_message_t *pmsg = encodeLoginRequestToProtobufMsg(msg);
  uint8_t *ret = ac_encode_protobuf_msg(pmsg, len);
  ac_protobuf_free_msg(pmsg);
  return ret;
}

void freeLoginRequest(struct LoginRequest *val) {
  free(val->username.value);
  free(val->password.value);
  free(val);
}
// clang-format on
