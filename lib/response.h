#ifndef IM_RESPONSE_H_
#define IM_RESPONSE_H_

#include <stdbool.h>

#include "proto/IMResponse.pb.h"
#include "proto/TextResponse.pb.h"
#include "proto/ExitResponse.pb.h"

struct IMResponse *encodeTextResponseToIMResponseAndFree(struct TextResponse *irsp, bool success);

struct IMResponse *encodeTextToIMResponse(char *txt, bool success);

struct IMResponse *encodeExitResponseToIMResponseAndFree(struct ExitResponse *irsp, bool success);

struct IMResponse *encodeExitTextToIMResponse(char *txt, bool success);

#endif
