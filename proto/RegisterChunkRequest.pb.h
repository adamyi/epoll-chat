// clang-format off
// this file is auto-generated with ac_protoc. Do not modify by hand.

#ifndef AC_HELPER_ACPROTOBUF_MSG_HEADER_RegisterChunkRequest
#define AC_HELPER_ACPROTOBUF_MSG_HEADER_RegisterChunkRequest

#include <stdbool.h>
#include <stdint.h>
#include "achelper/ac_protobuf.h"

struct RegisterChunkRequest {
  ac_protobuf_string_t filename;
  ac_protobuf_string_t chunks;
};

struct RegisterChunkRequest *parseRegisterChunkRequestFromProtobufMsg(ac_protobuf_message_t *msg);
struct RegisterChunkRequest *parseRegisterChunkRequestFromBytes(uint8_t *bytes, size_t len, size_t *read);
ac_protobuf_message_t *encodeRegisterChunkRequestToProtobufMsg(struct RegisterChunkRequest *msg);
uint8_t *encodeRegisterChunkRequestToBytes(struct RegisterChunkRequest *msg, size_t *len);
void freeRegisterChunkRequest(struct RegisterChunkRequest *val);

#endif
// clang-format on
