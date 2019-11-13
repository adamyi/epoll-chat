#include <memory.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "proto/ExitResponse.pb.h"
#include "proto/IMResponse.pb.h"
#include "proto/TextResponse.pb.h"

struct IMResponse *encodeTextResponseToIMResponseAndFree(
    struct TextResponse *irsp, bool success) {
  struct IMResponse *rsp = malloc(sizeof(struct IMResponse));
  rsp->type = 1;
  rsp->value.value = encodeTextResponseToBytes(irsp, &(rsp->value.len));
  freeTextResponse(irsp);

  rsp->success = success;
  return rsp;
}

struct IMResponse *encodeTextToIMResponse(char *txt, bool success) {
  struct TextResponse *irsp = malloc(sizeof(struct TextResponse));
  irsp->msg.len = strlen(txt);
  irsp->msg.value = malloc(irsp->msg.len + 1);
  memcpy(irsp->msg.value, txt, irsp->msg.len + 1);

  return encodeTextResponseToIMResponseAndFree(irsp, success);
}

struct IMResponse *encodeExitResponseToIMResponseAndFree(
    struct ExitResponse *irsp, bool success) {
  struct IMResponse *rsp = malloc(sizeof(struct IMResponse));
  rsp->type = 2;
  rsp->value.value = encodeExitResponseToBytes(irsp, &(rsp->value.len));
  freeExitResponse(irsp);

  rsp->success = success;
  return rsp;
}

struct IMResponse *encodeExitTextToIMResponse(char *txt, bool success) {
  struct ExitResponse *irsp = malloc(sizeof(struct ExitResponse));
  irsp->msg.len = strlen(txt);
  irsp->msg.value = malloc(irsp->msg.len + 1);
  memcpy(irsp->msg.value, txt, irsp->msg.len + 1);

  return encodeExitResponseToIMResponseAndFree(irsp, success);
}
