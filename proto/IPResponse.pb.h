// clang-format off
// this file is auto-generated with ac_protoc. Do not modify by hand.

#ifndef AC_HELPER_ACPROTOBUF_MSG_HEADER_IPResponse
#define AC_HELPER_ACPROTOBUF_MSG_HEADER_IPResponse

#include <stdbool.h>
#include <stdint.h>
#include "achelper/ac_protobuf.h"

struct IPResponse {
  ac_protobuf_string_t username;
  ac_protobuf_string_t ip;
  uint32_t port;
};

struct IPResponse *parseIPResponseFromProtobufMsg(ac_protobuf_message_t *msg);
struct IPResponse *parseIPResponseFromBytes(uint8_t *bytes, size_t len, size_t *read);
ac_protobuf_message_t *encodeIPResponseToProtobufMsg(struct IPResponse *msg);
uint8_t *encodeIPResponseToBytes(struct IPResponse *msg, size_t *len);
void freeIPResponse(struct IPResponse *val);

#endif
// clang-format on
