// clang-format off
// this file is auto-generated with ac_protoc. Do not modify by hand.

#include <memory.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "achelper/ac_protobuf.h"

#include "WhoElseRequest.pb.h"

struct WhoElseRequest *parseWhoElseRequestFromProtobufMsg(ac_protobuf_message_t *msg) {
  struct WhoElseRequest *ret = malloc(sizeof(struct WhoElseRequest));
  ac_protobuf_field_t *limitTime_f = ac_find_protobuf_field_in_msg(msg, 1);
  ret->limitTime = *(bool *)(limitTime_f->value);
  ac_protobuf_field_t *pastSeconds_f = ac_find_protobuf_field_in_msg(msg, 2);
  ret->pastSeconds = *(uint32_t *)(pastSeconds_f->value);
  ac_protobuf_field_t *currentlyOnline_f = ac_find_protobuf_field_in_msg(msg, 3);
  ret->currentlyOnline = *(bool *)(currentlyOnline_f->value);
  return ret;
}

struct WhoElseRequest *parseWhoElseRequestFromBytes(uint8_t *bytes, size_t len) {
  ac_protobuf_message_t *msg = ac_decode_protobuf_msg(bytes, len);
  struct WhoElseRequest *ret = parseWhoElseRequestFromProtobufMsg(msg);
  ac_protobuf_free_msg(msg);
  return ret;
}

ac_protobuf_message_t *encodeWhoElseRequestToProtobufMsg(struct WhoElseRequest *msg) {
  ac_protobuf_message_t *ret = malloc(sizeof(ac_protobuf_message_t));
  ac_protobuf_field_t *limitTime_f = malloc(sizeof(ac_protobuf_field_t));
  limitTime_f->id = 1;
  ret->fields = limitTime_f;
  limitTime_f->wiretype = 0;
  limitTime_f->value = malloc(8);
  *(uint64_t *)(limitTime_f->value) = (uint64_t)(msg->limitTime);
  ac_protobuf_field_t *pastSeconds_f = malloc(sizeof(ac_protobuf_field_t));
  pastSeconds_f->id = 2;
  limitTime_f->next = pastSeconds_f;
  pastSeconds_f->wiretype = 0;
  pastSeconds_f->value = malloc(8);
  *(uint64_t *)(pastSeconds_f->value) = (uint64_t)(msg->pastSeconds);
  ac_protobuf_field_t *currentlyOnline_f = malloc(sizeof(ac_protobuf_field_t));
  currentlyOnline_f->id = 3;
  pastSeconds_f->next = currentlyOnline_f;
  currentlyOnline_f->next = NULL;
  currentlyOnline_f->wiretype = 0;
  currentlyOnline_f->value = malloc(8);
  *(uint64_t *)(currentlyOnline_f->value) = (uint64_t)(msg->currentlyOnline);
  return ret;
}

uint8_t *encodeWhoElseRequestToBytes(struct WhoElseRequest *msg, size_t *len) {
  ac_protobuf_message_t *pmsg = encodeWhoElseRequestToProtobufMsg(msg);
  uint8_t *ret = ac_encode_protobuf_msg(pmsg, len);
  ac_protobuf_free_msg(pmsg);
  return ret;
}

void freeWhoElseRequest(struct WhoElseRequest *val) {
  free(val);
}
// clang-format on
