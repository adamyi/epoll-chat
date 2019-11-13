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
  ac_protobuf_field_t *type_f = ac_find_protobuf_field_in_msg(msg, 1);
  ret->type = *(uint32_t *)(type_f->value);
  ac_protobuf_field_t *success_f = ac_find_protobuf_field_in_msg(msg, 2);
  ret->success = *(bool *)(success_f->value);
  ac_protobuf_field_t *value_f = ac_find_protobuf_field_in_msg(msg, 3);
  ret->value.value = malloc(value_f->len + 1);
  memcpy(ret->value.value, value_f->value, value_f->len);
  ret->value.value[value_f->len] = 0;
  ret->value.len = value_f->len;
  return ret;
}

struct IMResponse *parseIMResponseFromBytes(uint8_t *bytes, size_t len, size_t *read) {
  ac_protobuf_message_t *msg = ac_decode_protobuf_msg(bytes, len, read);
  struct IMResponse *ret = parseIMResponseFromProtobufMsg(msg);
  ac_protobuf_free_msg(msg);
  return ret;
}

ac_protobuf_message_t *encodeIMResponseToProtobufMsg(struct IMResponse *msg) {
  ac_protobuf_message_t *ret = malloc(sizeof(ac_protobuf_message_t));
  ac_protobuf_field_t *type_f = malloc(sizeof(ac_protobuf_field_t));
  type_f->id = 1;
  ret->fields = type_f;
  type_f->wiretype = 0;
  type_f->value = malloc(8);
  *(uint64_t *)(type_f->value) = (uint64_t)(msg->type);
  ac_protobuf_field_t *success_f = malloc(sizeof(ac_protobuf_field_t));
  success_f->id = 2;
  type_f->next = success_f;
  success_f->wiretype = 0;
  success_f->value = malloc(8);
  *(uint64_t *)(success_f->value) = (uint64_t)(msg->success);
  ac_protobuf_field_t *value_f = malloc(sizeof(ac_protobuf_field_t));
  value_f->id = 3;
  success_f->next = value_f;
  value_f->next = NULL;
  value_f->wiretype = 2;
  value_f->len = msg->value.len;
  value_f->value = malloc(value_f->len + 1);
  memcpy(value_f->value, msg->value.value, value_f->len);
  *(uint8_t *)(value_f->value + value_f->len) = 0;
  return ret;
}

uint8_t *encodeIMResponseToBytes(struct IMResponse *msg, size_t *len) {
  ac_protobuf_message_t *pmsg = encodeIMResponseToProtobufMsg(msg);
  uint8_t *ret = ac_encode_protobuf_msg(pmsg, len);
  ac_protobuf_free_msg(pmsg);
  return ret;
}

void freeIMResponse(struct IMResponse *val) {
  if (val->value.len > 0)
    free(val->value.value);
  free(val);
}
// clang-format on
