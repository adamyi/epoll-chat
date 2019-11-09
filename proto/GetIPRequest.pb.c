// clang-format off
// this file is auto-generated with ac_protoc. Do not modify by hand.

#include <memory.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "achelper/ac_protobuf.h"

#include "GetIPRequest.pb.h"

struct GetIPRequest *parseGetIPRequestFromProtobufMsg(ac_protobuf_message_t *msg) {
  struct GetIPRequest *ret = malloc(sizeof(struct GetIPRequest));
  ac_protobuf_field_t *username_f = ac_find_protobuf_field_in_msg(msg, 1);
  ret->username.value = malloc(username_f->len + 1);
  memcpy(ret->username.value, username_f->value, username_f->len);
  ret->username.value[username_f->len] = 0;
  ret->username.len = username_f->len;
  return ret;
}

struct GetIPRequest *parseGetIPRequestFromBytes(uint8_t *bytes, size_t len, size_t *read) {
  ac_protobuf_message_t *msg = ac_decode_protobuf_msg(bytes, len, read);
  struct GetIPRequest *ret = parseGetIPRequestFromProtobufMsg(msg);
  ac_protobuf_free_msg(msg);
  return ret;
}

ac_protobuf_message_t *encodeGetIPRequestToProtobufMsg(struct GetIPRequest *msg) {
  ac_protobuf_message_t *ret = malloc(sizeof(ac_protobuf_message_t));
  ac_protobuf_field_t *username_f = malloc(sizeof(ac_protobuf_field_t));
  username_f->id = 1;
  ret->fields = username_f;
  username_f->next = NULL;
  username_f->wiretype = 2;
  username_f->len = msg->username.len;
  username_f->value = malloc(username_f->len + 1);
  memcpy(username_f->value, msg->username.value, username_f->len);
  *(uint8_t *)(username_f->value + username_f->len) = 0;
  return ret;
}

uint8_t *encodeGetIPRequestToBytes(struct GetIPRequest *msg, size_t *len) {
  ac_protobuf_message_t *pmsg = encodeGetIPRequestToProtobufMsg(msg);
  uint8_t *ret = ac_encode_protobuf_msg(pmsg, len);
  ac_protobuf_free_msg(pmsg);
  return ret;
}

void freeGetIPRequest(struct GetIPRequest *val) {
  free(val->username.value);
  free(val);
}
// clang-format on
