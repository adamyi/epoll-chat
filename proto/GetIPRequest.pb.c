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
  ret->username.value = malloc(username_f->len);
  memcpy(ret->username.value, username_f->value, username_f->len);
  ret->username.len = username_f->len;
  return ret;
}

struct GetIPRequest *parseGetIPRequestFromBytes(uint8_t *bytes, size_t len) {
  ac_protobuf_message_t *msg = ac_decode_protobuf_msg(bytes, len);
  struct GetIPRequest *ret = parseGetIPRequestFromProtobufMsg(msg);
  ac_protobuf_free_msg(msg);
  return ret;
}

void freeGetIPRequest(struct GetIPRequest *val) {
  free(val->username.value);
  free(val);
}
// clang-format on
