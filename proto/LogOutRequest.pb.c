// clang-format off
// this file is auto-generated with ac_protoc. Do not modify by hand.

#include <memory.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "achelper/ac_protobuf.h"

#include "LogOutRequest.pb.h"

struct LogOutRequest *parseLogOutRequestFromProtobufMsg(ac_protobuf_message_t *msg) {
  struct LogOutRequest *ret = malloc(sizeof(struct LogOutRequest));
  return ret;
}

struct LogOutRequest *parseLogOutRequestFromBytes(uint8_t *bytes, size_t len, size_t *read) {
  ac_protobuf_message_t *msg = ac_decode_protobuf_msg(bytes, len, read);
  struct LogOutRequest *ret = parseLogOutRequestFromProtobufMsg(msg);
  ac_protobuf_free_msg(msg);
  return ret;
}

ac_protobuf_message_t *encodeLogOutRequestToProtobufMsg(struct LogOutRequest *msg) {
  ac_protobuf_message_t *ret = malloc(sizeof(ac_protobuf_message_t));
  ret->fields = NULL;
  return ret;
}

uint8_t *encodeLogOutRequestToBytes(struct LogOutRequest *msg, size_t *len) {
  ac_protobuf_message_t *pmsg = encodeLogOutRequestToProtobufMsg(msg);
  uint8_t *ret = ac_encode_protobuf_msg(pmsg, len);
  ac_protobuf_free_msg(pmsg);
  return ret;
}

void freeLogOutRequest(struct LogOutRequest *val) {
  free(val);
}
// clang-format on
