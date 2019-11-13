// clang-format off
// this file is auto-generated with ac_protoc. Do not modify by hand.

#ifndef AC_HELPER_ACPROTOBUF_MSG_HEADER_ExitResponse
#define AC_HELPER_ACPROTOBUF_MSG_HEADER_ExitResponse

#include <stdbool.h>
#include <stdint.h>
#include "achelper/ac_protobuf.h"

struct ExitResponse {
  ac_protobuf_string_t msg;
};

struct ExitResponse *parseExitResponseFromProtobufMsg(ac_protobuf_message_t *msg);
struct ExitResponse *parseExitResponseFromBytes(uint8_t *bytes, size_t len, size_t *read);
ac_protobuf_message_t *encodeExitResponseToProtobufMsg(struct ExitResponse *msg);
uint8_t *encodeExitResponseToBytes(struct ExitResponse *msg, size_t *len);
void freeExitResponse(struct ExitResponse *val);

#endif
// clang-format on
