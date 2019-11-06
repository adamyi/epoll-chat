// clang-format off
// this file is auto-generated with ac_protoc. Do not modify by hand.

#include <memory.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "achelper/ac_protobuf.h"

#include "MessageRequest.pb.h"

struct MessageRequest *parseMessageRequestFromProtobufMsg(ac_protobuf_message_t *msg) {
  struct MessageRequest *ret = malloc(sizeof(struct MessageRequest));
  ac_protobuf_field_t *broadcast_f = ac_find_protobuf_field_in_msg(msg, 1);
  ret->broadcast = *(bool *)(broadcast_f->value);
  ac_protobuf_field_t *username_f = ac_find_protobuf_field_in_msg(msg, 2);
  ret->username.value = malloc(username_f->len);
  memcpy(ret->username.value, username_f->value, username_f->len);
  ret->username.len = username_f->len;
  ac_protobuf_field_t *msg_f = ac_find_protobuf_field_in_msg(msg, 3);
  ret->msg.value = malloc(msg_f->len);
  memcpy(ret->msg.value, msg_f->value, msg_f->len);
  ret->msg.len = msg_f->len;
  return ret;
}

struct MessageRequest *parseMessageRequestFromBytes(uint8_t *bytes, size_t len) {
  ac_protobuf_message_t *msg = ac_decode_protobuf_msg(bytes, len);
  struct MessageRequest *ret = parseMessageRequestFromProtobufMsg(msg);
  ac_protobuf_free_msg(msg);
  return ret;
}

void freeMessageRequest(struct MessageRequest *val) {
  free(val->username.value);
  free(val->msg.value);
  free(val);
}
// clang-format on
