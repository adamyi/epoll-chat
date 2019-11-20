// clang-format off
// this file is auto-generated with ac_protoc. Do not modify by hand.

#ifndef AC_HELPER_ACPROTOBUF_MSG_HEADER_ChunkDataResponse
#define AC_HELPER_ACPROTOBUF_MSG_HEADER_ChunkDataResponse

#include <stdbool.h>
#include <stdint.h>
#include "achelper/ac_protobuf.h"

struct ChunkDataResponse {
  ac_protobuf_string_t filename;
  uint32_t chunk;
  ac_protobuf_string_t data;
};

struct ChunkDataResponse *parseChunkDataResponseFromProtobufMsg(ac_protobuf_message_t *msg);
struct ChunkDataResponse *parseChunkDataResponseFromBytes(uint8_t *bytes, size_t len, size_t *read);
ac_protobuf_message_t *encodeChunkDataResponseToProtobufMsg(struct ChunkDataResponse *msg);
uint8_t *encodeChunkDataResponseToBytes(struct ChunkDataResponse *msg, size_t *len);
void freeChunkDataResponse(struct ChunkDataResponse *val);

#endif
// clang-format on
