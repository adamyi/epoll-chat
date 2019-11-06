// clang-format off
// this file is auto-generated with ac_protoc. Do not modify by hand.

#ifndef AC_HELPER_ACPROTOBUF_MSG_HEADER_WhoElseRequest
#define AC_HELPER_ACPROTOBUF_MSG_HEADER_WhoElseRequest

#include <stdbool.h>
#include <stdint.h>
#include "achelper/ac_protobuf.h"

struct WhoElseRequest {
  bool limitTime;
  uint32_t pastSeconds;
  bool currentlyOnline;
};

struct WhoElseRequest *parseWhoElseRequestFromProtobufMsg(ac_protobuf_message_t *msg);
struct WhoElseRequest *parseWhoElseRequestFromBytes(uint8_t *bytes, size_t len);
ac_protobuf_message_t *encodeWhoElseRequestToProtobufMsg(struct WhoElseRequest *msg);
uint8_t *encodeWhoElseRequestToBytes(struct WhoElseRequest *msg, size_t *len);
void freeWhoElseRequest(struct WhoElseRequest *val);

#endif
// clang-format on
