// clang-format off
// this file is auto-generated with ac_protoc. Do not modify by hand.

#ifndef AC_HELPER_ACPROTOBUF_MSG_HEADER_MessageRequest
#define AC_HELPER_ACPROTOBUF_MSG_HEADER_MessageRequest

#include <stdbool.h>
#include <stdint.h>
#include "achelper/ac_protobuf.h"

struct MessageRequest {
  bool broadcast;
  ac_protobuf_string_t username;
  ac_protobuf_string_t msg;
};

struct MessageRequest *parseMessageRequestFromProtobufMsg(ac_protobuf_message_t *msg);
struct MessageRequest *parseMessageRequestFromBytes(uint8_t *bytes, size_t len);
void freeMessageRequest(struct MessageRequest *val);

#endif
// clang-format on
