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
  ret->username.value = malloc(username_f->len + 1);
  memcpy(ret->username.value, username_f->value, username_f->len);
  ret->username.value[username_f->len] = 0;
  ret->username.len = username_f->len;
  ac_protobuf_field_t *msg_f = ac_find_protobuf_field_in_msg(msg, 3);
  ret->msg.value = malloc(msg_f->len + 1);
  memcpy(ret->msg.value, msg_f->value, msg_f->len);
  ret->msg.value[msg_f->len] = 0;
  ret->msg.len = msg_f->len;
  return ret;
}

struct MessageRequest *parseMessageRequestFromBytes(uint8_t *bytes, size_t len, size_t *read) {
  ac_protobuf_message_t *msg = ac_decode_protobuf_msg(bytes, len, read);
  struct MessageRequest *ret = parseMessageRequestFromProtobufMsg(msg);
  ac_protobuf_free_msg(msg);
  return ret;
}

ac_protobuf_message_t *encodeMessageRequestToProtobufMsg(struct MessageRequest *msg) {
  ac_protobuf_message_t *ret = malloc(sizeof(ac_protobuf_message_t));
  ac_protobuf_field_t *broadcast_f = malloc(sizeof(ac_protobuf_field_t));
  broadcast_f->id = 1;
  ret->fields = broadcast_f;
  broadcast_f->wiretype = 0;
  broadcast_f->value = malloc(8);
  *(uint64_t *)(broadcast_f->value) = (uint64_t)(msg->broadcast);
  ac_protobuf_field_t *username_f = malloc(sizeof(ac_protobuf_field_t));
  username_f->id = 2;
  broadcast_f->next = username_f;
  username_f->wiretype = 2;
  username_f->len = msg->username.len;
  username_f->value = malloc(username_f->len + 1);
  memcpy(username_f->value, msg->username.value, username_f->len);
  *(uint8_t *)(username_f->value + username_f->len) = 0;
  ac_protobuf_field_t *msg_f = malloc(sizeof(ac_protobuf_field_t));
  msg_f->id = 3;
  username_f->next = msg_f;
  msg_f->next = NULL;
  msg_f->wiretype = 2;
  msg_f->len = msg->msg.len;
  msg_f->value = malloc(msg_f->len + 1);
  memcpy(msg_f->value, msg->msg.value, msg_f->len);
  *(uint8_t *)(msg_f->value + msg_f->len) = 0;
  return ret;
}

uint8_t *encodeMessageRequestToBytes(struct MessageRequest *msg, size_t *len) {
  ac_protobuf_message_t *pmsg = encodeMessageRequestToProtobufMsg(msg);
  uint8_t *ret = ac_encode_protobuf_msg(pmsg, len);
  ac_protobuf_free_msg(pmsg);
  return ret;
}

void freeMessageRequest(struct MessageRequest *val) {
  if (val->username.len > 0)
    free(val->username.value);
  if (val->msg.len > 0)
    free(val->msg.value);
  free(val);
}
// clang-format on
