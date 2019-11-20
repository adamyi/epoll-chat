// clang-format off
// this file is auto-generated with ac_protoc. Do not modify by hand.

#ifndef AC_HELPER_ACPROTOBUF_MSG_HEADER_RegisterFileRequest
#define AC_HELPER_ACPROTOBUF_MSG_HEADER_RegisterFileRequest

#include <stdbool.h>
#include <stdint.h>
#include "achelper/ac_protobuf.h"

struct RegisterFileRequest {
  ac_protobuf_string_t filename;
  uint32_t chunks;
};

struct RegisterFileRequest *parseRegisterFileRequestFromProtobufMsg(ac_protobuf_message_t *msg);
struct RegisterFileRequest *parseRegisterFileRequestFromBytes(uint8_t *bytes, size_t len, size_t *read);
ac_protobuf_message_t *encodeRegisterFileRequestToProtobufMsg(struct RegisterFileRequest *msg);
uint8_t *encodeRegisterFileRequestToBytes(struct RegisterFileRequest *msg, size_t *len);
void freeRegisterFileRequest(struct RegisterFileRequest *val);

#endif
// clang-format on
