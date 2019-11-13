// clang-format off
// this file is auto-generated with ac_protoc. Do not modify by hand.

#ifndef AC_HELPER_ACPROTOBUF_MSG_HEADER_TextResponse
#define AC_HELPER_ACPROTOBUF_MSG_HEADER_TextResponse

#include <stdbool.h>
#include <stdint.h>
#include "achelper/ac_protobuf.h"

struct TextResponse {
  ac_protobuf_string_t msg;
};

struct TextResponse *parseTextResponseFromProtobufMsg(ac_protobuf_message_t *msg);
struct TextResponse *parseTextResponseFromBytes(uint8_t *bytes, size_t len, size_t *read);
ac_protobuf_message_t *encodeTextResponseToProtobufMsg(struct TextResponse *msg);
uint8_t *encodeTextResponseToBytes(struct TextResponse *msg, size_t *len);
void freeTextResponse(struct TextResponse *val);

#endif
// clang-format on
