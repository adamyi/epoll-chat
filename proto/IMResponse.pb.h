// clang-format off
// this file is auto-generated with ac_protoc. Do not modify by hand.

#ifndef AC_HELPER_ACPROTOBUF_MSG_HEADER_IMResponse
#define AC_HELPER_ACPROTOBUF_MSG_HEADER_IMResponse

#include <stdbool.h>
#include <stdint.h>
#include "achelper/ac_protobuf.h"

struct IMResponse {
  bool success;
  ac_protobuf_string_t msg;
};

struct IMResponse *parseIMResponseFromProtobufMsg(ac_protobuf_message_t *msg);
struct IMResponse *parseIMResponseFromBytes(uint8_t *bytes, size_t len, size_t *read);
ac_protobuf_message_t *encodeIMResponseToProtobufMsg(struct IMResponse *msg);
uint8_t *encodeIMResponseToBytes(struct IMResponse *msg, size_t *len);
void freeIMResponse(struct IMResponse *val);

#endif
// clang-format on
