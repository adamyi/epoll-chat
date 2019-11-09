// clang-format off
// this file is auto-generated with ac_protoc. Do not modify by hand.

#ifndef AC_HELPER_ACPROTOBUF_MSG_HEADER_GetIPRequest
#define AC_HELPER_ACPROTOBUF_MSG_HEADER_GetIPRequest

#include <stdbool.h>
#include <stdint.h>
#include "achelper/ac_protobuf.h"

struct GetIPRequest {
  ac_protobuf_string_t username;
};

struct GetIPRequest *parseGetIPRequestFromProtobufMsg(ac_protobuf_message_t *msg);
struct GetIPRequest *parseGetIPRequestFromBytes(uint8_t *bytes, size_t len, size_t *read);
ac_protobuf_message_t *encodeGetIPRequestToProtobufMsg(struct GetIPRequest *msg);
uint8_t *encodeGetIPRequestToBytes(struct GetIPRequest *msg, size_t *len);
void freeGetIPRequest(struct GetIPRequest *val);

#endif
// clang-format on
