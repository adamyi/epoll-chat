// clang-format off
// this file is auto-generated with ac_protoc. Do not modify by hand.

#ifndef AC_HELPER_ACPROTOBUF_MSG_HEADER_IMRequest
#define AC_HELPER_ACPROTOBUF_MSG_HEADER_IMRequest

#include <stdbool.h>
#include <stdint.h>
#include "achelper/ac_protobuf.h"

struct IMRequest {
  uint32_t type;
  ac_protobuf_string_t value;
};

struct IMRequest *parseIMRequestFromProtobufMsg(ac_protobuf_message_t *msg);
struct IMRequest *parseIMRequestFromBytes(uint8_t *bytes, size_t len, size_t *read);
ac_protobuf_message_t *encodeIMRequestToProtobufMsg(struct IMRequest *msg);
uint8_t *encodeIMRequestToBytes(struct IMRequest *msg, size_t *len);
void freeIMRequest(struct IMRequest *val);

#endif
// clang-format on
