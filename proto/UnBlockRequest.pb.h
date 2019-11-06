// clang-format off
// this file is auto-generated with ac_protoc. Do not modify by hand.

#ifndef AC_HELPER_ACPROTOBUF_MSG_HEADER_UnBlockRequest
#define AC_HELPER_ACPROTOBUF_MSG_HEADER_UnBlockRequest

#include <stdbool.h>
#include <stdint.h>
#include "achelper/ac_protobuf.h"

struct UnBlockRequest {
  ac_protobuf_string_t username;
};

struct UnBlockRequest *parseUnBlockRequestFromProtobufMsg(ac_protobuf_message_t *msg);
struct UnBlockRequest *parseUnBlockRequestFromBytes(uint8_t *bytes, size_t len);
ac_protobuf_message_t *encodeUnBlockRequestToProtobufMsg(struct UnBlockRequest *msg);
uint8_t *encodeUnBlockRequestToBytes(struct UnBlockRequest *msg, size_t *len);
void freeUnBlockRequest(struct UnBlockRequest *val);

#endif
// clang-format on
