#ifndef IM_COMMAND_H_
#define IM_COMMAND_H_

#include "client.h"
// do not sort
#include "auth.h"

#include "proto/IMResponse.pb.h"

typedef struct im_command {
  uint32_t type;
  char *prefix;
  struct IMRequest *(*run)(int epollfd, im_client_t *client, char *req);
} im_command_t;

void parse_command(int epollfd, im_client_t *client, uint8_t *cmd, size_t len,
                   struct IMResponse **rsp);

size_t parse_response(int epollfd, im_client_t *client, uint8_t *cmd,
                      size_t len, struct IMResponse **rsp);

extern const im_command_t *enabled_commands[];

#endif
