#ifndef IM_COMMAND_H_
#define IM_COMMAND_H_

#include "lib/client.h"
// do not sort
#include "lib/auth.h"
#include "lib/buffer.h"

#include "proto/IMRequest.pb.h"
#include "proto/IMResponse.pb.h"

typedef struct im_command {
  char *prefix;
  struct IMRequest *(*run)(int epollfd, im_client_t *client, char *req);
} im_command_t;

void send_request(im_buffer_t *buffer, struct IMRequest *msg);

void parse_command(int epollfd, im_client_t *client, uint8_t *command,
                   size_t len);

extern const im_command_t *enabled_commands[];

#endif
