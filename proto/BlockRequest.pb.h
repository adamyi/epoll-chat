// clang-format off
// this file is auto-generated with ac_protoc. Do not modify by hand.

#ifndef AC_HELPER_ACPROTOBUF_MSG_HEADER_BlockRequest
#define AC_HELPER_ACPROTOBUF_MSG_HEADER_BlockRequest

#include <stdbool.h>
#include <stdint.h>
#include "achelper/ac_protobuf.h"

struct BlockRequest {
  ac_protobuf_string_t username;
};

struct BlockRequest *parseBlockRequestFromProtobufMsg(ac_protobuf_message_t *msg);
struct BlockRequest *parseBlockRequestFromBytes(uint8_t *bytes, size_t len);
void freeBlockRequest(struct BlockRequest *val);

#endif
// clang-format on
