// clang-format off
// this file is auto-generated with ac_protoc. Do not modify by hand.

#ifndef AC_HELPER_ACPROTOBUF_MSG_HEADER_LoginRequest
#define AC_HELPER_ACPROTOBUF_MSG_HEADER_LoginRequest

#include <stdbool.h>
#include <stdint.h>
#include "achelper/ac_protobuf.h"

struct LoginRequest {
  ac_protobuf_string_t username;
  ac_protobuf_string_t password;
};

struct LoginRequest *parseLoginRequestFromProtobufMsg(ac_protobuf_message_t *msg);
struct LoginRequest *parseLoginRequestFromBytes(uint8_t *bytes, size_t len);
void freeLoginRequest(struct LoginRequest *val);

#endif
// clang-format on
