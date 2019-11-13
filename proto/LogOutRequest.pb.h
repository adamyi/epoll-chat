// clang-format off
// this file is auto-generated with ac_protoc. Do not modify by hand.

#ifndef AC_HELPER_ACPROTOBUF_MSG_HEADER_LogOutRequest
#define AC_HELPER_ACPROTOBUF_MSG_HEADER_LogOutRequest

#include <stdbool.h>
#include <stdint.h>
#include "achelper/ac_protobuf.h"

struct LogOutRequest {
};

struct LogOutRequest *parseLogOutRequestFromProtobufMsg(ac_protobuf_message_t *msg);
struct LogOutRequest *parseLogOutRequestFromBytes(uint8_t *bytes, size_t len, size_t *read);
ac_protobuf_message_t *encodeLogOutRequestToProtobufMsg(struct LogOutRequest *msg);
uint8_t *encodeLogOutRequestToBytes(struct LogOutRequest *msg, size_t *len);
void freeLogOutRequest(struct LogOutRequest *val);

#endif
// clang-format on
