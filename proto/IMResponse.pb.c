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
  ret->msg.value = malloc(msg_f->len + 1);
  memcpy(ret->msg.value, msg_f->value, msg_f->len);
  ret->msg.value[msg_f->len] = 0;
  ret->msg.len = msg_f->len;
  return ret;
}

struct IMResponse *parseIMResponseFromBytes(uint8_t *bytes, size_t len) {
  ac_protobuf_message_t *msg = ac_decode_protobuf_msg(bytes, len);
  struct IMResponse *ret = parseIMResponseFromProtobufMsg(msg);
  ac_protobuf_free_msg(msg);
  return ret;
}

ac_protobuf_message_t *encodeIMResponseToProtobufMsg(struct IMResponse *msg) {
  ac_protobuf_message_t *ret = malloc(sizeof(ac_protobuf_message_t));
  ac_protobuf_field_t *success_f = malloc(sizeof(ac_protobuf_field_t));
  success_f->id = 1;
  ret->fields = success_f;
  success_f->wiretype = 0;
  success_f->value = malloc(8);
  *(uint64_t *)(success_f->value) = (uint64_t)(msg->success);
  ac_protobuf_field_t *msg_f = malloc(sizeof(ac_protobuf_field_t));
  msg_f->id = 2;
  success_f->next = msg_f;
  msg_f->next = NULL;
  msg_f->wiretype = 2;
  msg_f->len = msg->msg.len;
  msg_f->value = malloc(msg_f->len + 1);
  memcpy(msg_f->value, msg->msg.value, msg_f->len);
  *(uint8_t *)(msg_f->value + msg_f->len) = 0;
  return ret;
}

uint8_t *encodeIMResponseToBytes(struct IMResponse *msg, size_t *len) {
  ac_protobuf_message_t *pmsg = encodeIMResponseToProtobufMsg(msg);
  uint8_t *ret = ac_encode_protobuf_msg(pmsg, len);
  ac_protobuf_free_msg(pmsg);
  return ret;
}

void freeIMResponse(struct IMResponse *val) {
  free(val->msg.value);
  free(val);
}
// clang-format on
