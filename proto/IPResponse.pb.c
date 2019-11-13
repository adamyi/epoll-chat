// clang-format off
// this file is auto-generated with ac_protoc. Do not modify by hand.

#include <memory.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "achelper/ac_protobuf.h"

#include "IPResponse.pb.h"

struct IPResponse *parseIPResponseFromProtobufMsg(ac_protobuf_message_t *msg) {
  struct IPResponse *ret = malloc(sizeof(struct IPResponse));
  ac_protobuf_field_t *username_f = ac_find_protobuf_field_in_msg(msg, 1);
  ret->username.value = malloc(username_f->len + 1);
  memcpy(ret->username.value, username_f->value, username_f->len);
  ret->username.value[username_f->len] = 0;
  ret->username.len = username_f->len;
  ac_protobuf_field_t *ip_f = ac_find_protobuf_field_in_msg(msg, 2);
  ret->ip.value = malloc(ip_f->len + 1);
  memcpy(ret->ip.value, ip_f->value, ip_f->len);
  ret->ip.value[ip_f->len] = 0;
  ret->ip.len = ip_f->len;
  ac_protobuf_field_t *port_f = ac_find_protobuf_field_in_msg(msg, 3);
  ret->port = *(uint32_t *)(port_f->value);
  return ret;
}

struct IPResponse *parseIPResponseFromBytes(uint8_t *bytes, size_t len, size_t *read) {
  ac_protobuf_message_t *msg = ac_decode_protobuf_msg(bytes, len, read);
  struct IPResponse *ret = parseIPResponseFromProtobufMsg(msg);
  ac_protobuf_free_msg(msg);
  return ret;
}

ac_protobuf_message_t *encodeIPResponseToProtobufMsg(struct IPResponse *msg) {
  ac_protobuf_message_t *ret = malloc(sizeof(ac_protobuf_message_t));
  ac_protobuf_field_t *username_f = malloc(sizeof(ac_protobuf_field_t));
  username_f->id = 1;
  ret->fields = username_f;
  username_f->wiretype = 2;
  username_f->len = msg->username.len;
  username_f->value = malloc(username_f->len + 1);
  memcpy(username_f->value, msg->username.value, username_f->len);
  *(uint8_t *)(username_f->value + username_f->len) = 0;
  ac_protobuf_field_t *ip_f = malloc(sizeof(ac_protobuf_field_t));
  ip_f->id = 2;
  username_f->next = ip_f;
  ip_f->wiretype = 2;
  ip_f->len = msg->ip.len;
  ip_f->value = malloc(ip_f->len + 1);
  memcpy(ip_f->value, msg->ip.value, ip_f->len);
  *(uint8_t *)(ip_f->value + ip_f->len) = 0;
  ac_protobuf_field_t *port_f = malloc(sizeof(ac_protobuf_field_t));
  port_f->id = 3;
  ip_f->next = port_f;
  port_f->next = NULL;
  port_f->wiretype = 0;
  port_f->value = malloc(8);
  *(uint64_t *)(port_f->value) = (uint64_t)(msg->port);
  return ret;
}

uint8_t *encodeIPResponseToBytes(struct IPResponse *msg, size_t *len) {
  ac_protobuf_message_t *pmsg = encodeIPResponseToProtobufMsg(msg);
  uint8_t *ret = ac_encode_protobuf_msg(pmsg, len);
  ac_protobuf_free_msg(pmsg);
  return ret;
}

void freeIPResponse(struct IPResponse *val) {
  if (val->username.len > 0)
    free(val->username.value);
  if (val->ip.len > 0)
    free(val->ip.value);
  free(val);
}
// clang-format on
