// clang-format off
// this file is auto-generated with ac_protoc. Do not modify by hand.

#include <memory.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "achelper/ac_protobuf.h"

#include "SearchFileRequest.pb.h"

struct SearchFileRequest *parseSearchFileRequestFromProtobufMsg(ac_protobuf_message_t *msg) {
  struct SearchFileRequest *ret = malloc(sizeof(struct SearchFileRequest));
  ac_protobuf_field_t *filename_f = ac_find_protobuf_field_in_msg(msg, 1);
  ret->filename.value = malloc(filename_f->len + 1);
  memcpy(ret->filename.value, filename_f->value, filename_f->len);
  ret->filename.value[filename_f->len] = 0;
  ret->filename.len = filename_f->len;
  ac_protobuf_field_t *chunks_f = ac_find_protobuf_field_in_msg(msg, 2);
  ret->chunks.value = malloc(chunks_f->len + 1);
  memcpy(ret->chunks.value, chunks_f->value, chunks_f->len);
  ret->chunks.value[chunks_f->len] = 0;
  ret->chunks.len = chunks_f->len;
  return ret;
}

struct SearchFileRequest *parseSearchFileRequestFromBytes(uint8_t *bytes, size_t len, size_t *read) {
  ac_protobuf_message_t *msg = ac_decode_protobuf_msg(bytes, len, read);
  struct SearchFileRequest *ret = parseSearchFileRequestFromProtobufMsg(msg);
  ac_protobuf_free_msg(msg);
  return ret;
}

ac_protobuf_message_t *encodeSearchFileRequestToProtobufMsg(struct SearchFileRequest *msg) {
  ac_protobuf_message_t *ret = malloc(sizeof(ac_protobuf_message_t));
  ac_protobuf_field_t *filename_f = malloc(sizeof(ac_protobuf_field_t));
  filename_f->id = 1;
  ret->fields = filename_f;
  filename_f->wiretype = 2;
  filename_f->len = msg->filename.len;
  filename_f->value = malloc(filename_f->len + 1);
  memcpy(filename_f->value, msg->filename.value, filename_f->len);
  *(uint8_t *)(filename_f->value + filename_f->len) = 0;
  ac_protobuf_field_t *chunks_f = malloc(sizeof(ac_protobuf_field_t));
  chunks_f->id = 2;
  filename_f->next = chunks_f;
  chunks_f->next = NULL;
  chunks_f->wiretype = 2;
  chunks_f->len = msg->chunks.len;
  chunks_f->value = malloc(chunks_f->len + 1);
  memcpy(chunks_f->value, msg->chunks.value, chunks_f->len);
  *(uint8_t *)(chunks_f->value + chunks_f->len) = 0;
  return ret;
}

uint8_t *encodeSearchFileRequestToBytes(struct SearchFileRequest *msg, size_t *len) {
  ac_protobuf_message_t *pmsg = encodeSearchFileRequestToProtobufMsg(msg);
  uint8_t *ret = ac_encode_protobuf_msg(pmsg, len);
  ac_protobuf_free_msg(pmsg);
  return ret;
}

void freeSearchFileRequest(struct SearchFileRequest *val) {
  if (val->filename.len > 0)
    free(val->filename.value);
  if (val->chunks.len > 0)
    free(val->chunks.value);
  free(val);
}
// clang-format on
