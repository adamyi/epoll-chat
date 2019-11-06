// clang-format off
// this file is auto-generated with ac_protoc. Do not modify by hand.

#include <memory.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "achelper/ac_protobuf.h"

#include "IMRequest.pb.h"

struct IMRequest *parseIMRequestFromProtobufMsg(ac_protobuf_message_t *msg) {
  struct IMRequest *ret = malloc(sizeof(struct IMRequest));
  ac_protobuf_field_t *type_f = ac_find_protobuf_field_in_msg(msg, 1);
  ret->type = *(uint32_t *)(type_f->value);
  ac_protobuf_field_t *value_f = ac_find_protobuf_field_in_msg(msg, 2);
  ret->value.value = malloc(value_f->len + 1);
  memcpy(ret->value.value, value_f->value, value_f->len);
  ret->value.value[value_f->len] = 0;
  ret->value.len = value_f->len;
  return ret;
}

struct IMRequest *parseIMRequestFromBytes(uint8_t *bytes, size_t len) {
  ac_protobuf_message_t *msg = ac_decode_protobuf_msg(bytes, len);
  struct IMRequest *ret = parseIMRequestFromProtobufMsg(msg);
  ac_protobuf_free_msg(msg);
  return ret;
}

void freeIMRequest(struct IMRequest *val) {
  free(val->value.value);
  free(val);
}
// clang-format on
