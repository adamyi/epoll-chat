// clang-format off
// this file is auto-generated with ac_protoc. Do not modify by hand.

#include <memory.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "achelper/ac_protobuf.h"

#include "IMResponse.pb.h"

struct IMResponse *parseIMResponseFromProtobufMsg(ac_protobuf_message_t *msg) {
  struct IMResponse *ret = malloc(sizeof(struct IMResponse));
  ac_protobuf_field_t *success_f = ac_find_protobuf_field_in_msg(msg, 1);
  ret->success = *(bool *)(success_f->value);
  ac_protobuf_field_t *msg_f = ac_find_protobuf_field_in_msg(msg, 2);
  ret->msg.value = malloc(msg_f->len);
  memcpy(ret->msg.value, msg_f->value, msg_f->len);
  ret->msg.len = msg_f->len;
  return ret;
}

struct IMResponse *parseIMResponseFromBytes(uint8_t *bytes, size_t len) {
  ac_protobuf_message_t *msg = ac_decode_protobuf_msg(bytes, len);
  struct IMResponse *ret = parseIMResponseFromProtobufMsg(msg);
  ac_protobuf_free_msg(msg);
  return ret;
}

void freeIMResponse(struct IMResponse *val) {
  free(val->msg.value);
  free(val);
}
// clang-format on
