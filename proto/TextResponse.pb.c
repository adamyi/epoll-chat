// clang-format off
// this file is auto-generated with ac_protoc. Do not modify by hand.

#include <memory.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "achelper/ac_protobuf.h"

#include "TextResponse.pb.h"

struct TextResponse *parseTextResponseFromProtobufMsg(ac_protobuf_message_t *msg) {
  struct TextResponse *ret = malloc(sizeof(struct TextResponse));
  ac_protobuf_field_t *msg_f = ac_find_protobuf_field_in_msg(msg, 1);
  ret->msg.value = malloc(msg_f->len + 1);
  memcpy(ret->msg.value, msg_f->value, msg_f->len);
  ret->msg.value[msg_f->len] = 0;
  ret->msg.len = msg_f->len;
  return ret;
}

struct TextResponse *parseTextResponseFromBytes(uint8_t *bytes, size_t len, size_t *read) {
  ac_protobuf_message_t *msg = ac_decode_protobuf_msg(bytes, len, read);
  struct TextResponse *ret = parseTextResponseFromProtobufMsg(msg);
  ac_protobuf_free_msg(msg);
  return ret;
}

ac_protobuf_message_t *encodeTextResponseToProtobufMsg(struct TextResponse *msg) {
  ac_protobuf_message_t *ret = malloc(sizeof(ac_protobuf_message_t));
  ac_protobuf_field_t *msg_f = malloc(sizeof(ac_protobuf_field_t));
  msg_f->id = 1;
  ret->fields = msg_f;
  msg_f->next = NULL;
  msg_f->wiretype = 2;
  msg_f->len = msg->msg.len;
  msg_f->value = malloc(msg_f->len + 1);
  memcpy(msg_f->value, msg->msg.value, msg_f->len);
  *(uint8_t *)(msg_f->value + msg_f->len) = 0;
  return ret;
}

uint8_t *encodeTextResponseToBytes(struct TextResponse *msg, size_t *len) {
  ac_protobuf_message_t *pmsg = encodeTextResponseToProtobufMsg(msg);
  uint8_t *ret = ac_encode_protobuf_msg(pmsg, len);
  ac_protobuf_free_msg(pmsg);
  return ret;
}

void freeTextResponse(struct TextResponse *val) {
  if (val->msg.len > 0)
    free(val->msg.value);
  free(val);
}
// clang-format on
