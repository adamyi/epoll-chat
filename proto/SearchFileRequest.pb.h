// clang-format off
// this file is auto-generated with ac_protoc. Do not modify by hand.

#ifndef AC_HELPER_ACPROTOBUF_MSG_HEADER_SearchFileRequest
#define AC_HELPER_ACPROTOBUF_MSG_HEADER_SearchFileRequest

#include <stdbool.h>
#include <stdint.h>
#include "achelper/ac_protobuf.h"

struct SearchFileRequest {
  ac_protobuf_string_t filename;
  ac_protobuf_string_t chunks;
};

struct SearchFileRequest *parseSearchFileRequestFromProtobufMsg(ac_protobuf_message_t *msg);
struct SearchFileRequest *parseSearchFileRequestFromBytes(uint8_t *bytes, size_t len, size_t *read);
ac_protobuf_message_t *encodeSearchFileRequestToProtobufMsg(struct SearchFileRequest *msg);
uint8_t *encodeSearchFileRequestToBytes(struct SearchFileRequest *msg, size_t *len);
void freeSearchFileRequest(struct SearchFileRequest *val);

#endif
// clang-format on
