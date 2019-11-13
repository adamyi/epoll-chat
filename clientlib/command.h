#ifndef IM_COMMAND_H_
#define IM_COMMAND_H_

#include "lib/client.h"
// do not sort
#include "lib/auth.h"

#include "proto/IMResponse.pb.h"

typedef struct im_command {
  char *prefix;
  struct IMRequest *(*run)(int epollfd, im_client_t *client, char *req);
} im_command_t;

size_t parse_response(UserDb *db, int epollfd, im_client_t *client,
                      uint8_t *cmd, size_t len, struct IMResponse **rsp);

void parse_command(int epollfd, im_client_t *client, uint8_t *command,
                   size_t len);

extern const im_command_t *enabled_commands[];

#endif
