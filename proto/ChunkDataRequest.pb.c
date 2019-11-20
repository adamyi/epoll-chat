// clang-format off
// this file is auto-generated with ac_protoc. Do not modify by hand.

#include <memory.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "achelper/ac_protobuf.h"

#include "ChunkDataRequest.pb.h"

struct ChunkDataRequest *parseChunkDataRequestFromProtobufMsg(ac_protobuf_message_t *msg) {
  struct ChunkDataRequest *ret = malloc(sizeof(struct ChunkDataRequest));
  ac_protobuf_field_t *filename_f = ac_find_protobuf_field_in_msg(msg, 1);
  ret->filename.value = malloc(filename_f->len + 1);
  memcpy(ret->filename.value, filename_f->value, filename_f->len);
  ret->filename.value[filename_f->len] = 0;
  ret->filename.len = filename_f->len;
  ac_protobuf_field_t *chunk_f = ac_find_protobuf_field_in_msg(msg, 2);
  ret->chunk = *(uint32_t *)(chunk_f->value);
  return ret;
}

struct ChunkDataRequest *parseChunkDataRequestFromBytes(uint8_t *bytes, size_t len, size_t *read) {
  ac_protobuf_message_t *msg = ac_decode_protobuf_msg(bytes, len, read);
  struct ChunkDataRequest *ret = parseChunkDataRequestFromProtobufMsg(msg);
  ac_protobuf_free_msg(msg);
  return ret;
}

ac_protobuf_message_t *encodeChunkDataRequestToProtobufMsg(struct ChunkDataRequest *msg) {
  ac_protobuf_message_t *ret = malloc(sizeof(ac_protobuf_message_t));
  ac_protobuf_field_t *filename_f = malloc(sizeof(ac_protobuf_field_t));
  filename_f->id = 1;
  ret->fields = filename_f;
  filename_f->wiretype = 2;
  filename_f->len = msg->filename.len;
  filename_f->value = malloc(filename_f->len + 1);
  memcpy(filename_f->value, msg->filename.value, filename_f->len);
  *(uint8_t *)(filename_f->value + filename_f->len) = 0;
  ac_protobuf_field_t *chunk_f = malloc(sizeof(ac_protobuf_field_t));
  chunk_f->id = 2;
  filename_f->next = chunk_f;
  chunk_f->next = NULL;
  chunk_f->wiretype = 0;
  chunk_f->value = malloc(8);
  *(uint64_t *)(chunk_f->value) = (uint64_t)(msg->chunk);
  return ret;
}

uint8_t *encodeChunkDataRequestToBytes(struct ChunkDataRequest *msg, size_t *len) {
  ac_protobuf_message_t *pmsg = encodeChunkDataRequestToProtobufMsg(msg);
  uint8_t *ret = ac_encode_protobuf_msg(pmsg, len);
  ac_protobuf_free_msg(pmsg);
  return ret;
}

void freeChunkDataRequest(struct ChunkDataRequest *val) {
  if (val->filename.len > 0)
    free(val->filename.value);
  free(val);
}
// clang-format on
