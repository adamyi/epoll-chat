// clang-format off
// this file is auto-generated with ac_protoc. Do not modify by hand.

#ifndef AC_HELPER_ACPROTOBUF_MSG_HEADER_ChunkDataRequest
#define AC_HELPER_ACPROTOBUF_MSG_HEADER_ChunkDataRequest

#include <stdbool.h>
#include <stdint.h>
#include "achelper/ac_protobuf.h"

struct ChunkDataRequest {
  ac_protobuf_string_t filename;
  uint32_t chunk;
};

struct ChunkDataRequest *parseChunkDataRequestFromProtobufMsg(ac_protobuf_message_t *msg);
struct ChunkDataRequest *parseChunkDataRequestFromBytes(uint8_t *bytes, size_t len, size_t *read);
ac_protobuf_message_t *encodeChunkDataRequestToProtobufMsg(struct ChunkDataRequest *msg);
uint8_t *encodeChunkDataRequestToBytes(struct ChunkDataRequest *msg, size_t *len);
void freeChunkDataRequest(struct ChunkDataRequest *val);

#endif
// clang-format on
