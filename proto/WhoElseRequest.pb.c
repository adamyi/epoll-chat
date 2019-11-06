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

void freeWhoElseRequest(struct WhoElseRequest *val) {
  free(val);
}
// clang-format on
