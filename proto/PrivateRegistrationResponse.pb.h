// clang-format off
// this file is auto-generated with ac_protoc. Do not modify by hand.

#ifndef AC_HELPER_ACPROTOBUF_MSG_HEADER_PrivateRegistrationResponse
#define AC_HELPER_ACPROTOBUF_MSG_HEADER_PrivateRegistrationResponse

#include <stdbool.h>
#include <stdint.h>
#include "achelper/ac_protobuf.h"

struct PrivateRegistrationResponse {
  ac_protobuf_string_t username;
};

struct PrivateRegistrationResponse *parsePrivateRegistrationResponseFromProtobufMsg(ac_protobuf_message_t *msg);
struct PrivateRegistrationResponse *parsePrivateRegistrationResponseFromBytes(uint8_t *bytes, size_t len, size_t *read);
ac_protobuf_message_t *encodePrivateRegistrationResponseToProtobufMsg(struct PrivateRegistrationResponse *msg);
uint8_t *encodePrivateRegistrationResponseToBytes(struct PrivateRegistrationResponse *msg, size_t *len);
void freePrivateRegistrationResponse(struct PrivateRegistrationResponse *val);

#endif
// clang-format on
