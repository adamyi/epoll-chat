// clang-format off
// this file is auto-generated with ac_protoc. Do not modify by hand.

#include <memory.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "achelper/ac_protobuf.h"

#include "UnBlockRequest.pb.h"

struct UnBlockRequest *parseUnBlockRequestFromProtobufMsg(ac_protobuf_message_t *msg) {
  struct UnBlockRequest *ret = malloc(sizeof(struct UnBlockRequest));
  ac_protobuf_field_t *username_f = ac_find_protobuf_field_in_msg(msg, 1);
  ret->username.value = malloc(username_f->len);
  memcpy(ret->username.value, username_f->value, username_f->len);
  ret->username.len = username_f->len;
  return ret;
}

struct UnBlockRequest *parseUnBlockRequestFromBytes(uint8_t *bytes, size_t len) {
  ac_protobuf_message_t *msg = ac_decode_protobuf_msg(bytes, len);
  struct UnBlockRequest *ret = parseUnBlockRequestFromProtobufMsg(msg);
  ac_protobuf_free_msg(msg);
  return ret;
}

void freeUnBlockRequest(struct UnBlockRequest *val) {
  free(val->username.value);
  free(val);
}
// clang-format on
