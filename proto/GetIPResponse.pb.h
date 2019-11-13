// clang-format off
// this file is auto-generated with ac_protoc. Do not modify by hand.

#ifndef AC_HELPER_ACPROTOBUF_MSG_HEADER_GetIPResponse
#define AC_HELPER_ACPROTOBUF_MSG_HEADER_GetIPResponse

#include <stdbool.h>
#include <stdint.h>
#include "achelper/ac_protobuf.h"

struct GetIPResponse {
  ac_protobuf_string_t username;
  ac_protobuf_string_t addr;
};

struct GetIPResponse *parseGetIPResponseFromProtobufMsg(ac_protobuf_message_t *msg);
struct GetIPResponse *parseGetIPResponseFromBytes(uint8_t *bytes, size_t len, size_t *read);
ac_protobuf_message_t *encodeGetIPResponseToProtobufMsg(struct GetIPResponse *msg);
uint8_t *encodeGetIPResponseToBytes(struct GetIPResponse *msg, size_t *len);
void freeGetIPResponse(struct GetIPResponse *val);

#endif
// clang-format on
