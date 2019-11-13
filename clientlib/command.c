#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "achelper/ac_log.h"
#include "achelper/ac_protobuf.h"

#include "lib/client.h"
// do not sort
#include "lib/auth.h"
#include "lib/socket.h"

#include "proto/IMRequest.pb.h"
#include "proto/IMResponse.pb.h"

#include "clientlib/commands/block.h"
#include "clientlib/commands/broadcast.h"
#include "clientlib/commands/login.h"
#include "clientlib/commands/logout.h"
#include "clientlib/commands/message.h"
#include "clientlib/commands/unblock.h"
#include "clientlib/commands/whoelse.h"
#include "clientlib/commands/whoelsesince.h"

const im_command_t *enabled_commands[] = {
    &cmd_login,  &cmd_message,      &cmd_broadcast, &cmd_block, &cmd_unblock,
    &cmd_logout, &cmd_whoelsesince, &cmd_whoelse,   NULL};

void send_request(im_buffer_t *buffer, struct IMRequest *msg) {
  ac_log(AC_LOG_INFO, "sending response");
  size_t len;
  uint8_t *bytes = encodeIMRequestToBytes(msg, &len);
  for (int i = 0; i < len; i++) {
    printf("%x ", bytes[i]);
  }
  printf("\n");
  if (buffer->buffer_end + len >= buffer->buffer_capacity) {
    reset_buffer_start(buffer);
    if (buffer->buffer_end + len >= buffer->buffer_capacity) {
      while (buffer->buffer_end + len >= buffer->buffer_capacity) {
        buffer->buffer_capacity <<= 1;
      }
      buffer->buffer = realloc(buffer->buffer, buffer->buffer_capacity);
    }
  }
  memcpy(buffer->buffer + buffer->buffer_end, bytes, len);
  buffer->buffer_end += len;
}

void parse_command(int epollfd, im_client_t *client, uint8_t *command,
                   size_t len) {
  bool hasrun = false;
  for (const im_command_t **cmd = enabled_commands; *cmd != NULL; cmd++) {
    printf("checking %p\n", cmd);
    if (strlen((*cmd)->prefix) <= len &&
        strncmp((char *)((*cmd)->prefix), (char *)command, strlen((*cmd)->prefix)) == 0) {
      ac_log(AC_LOG_DEBUG, "found implementation for command %s",
             (*cmd)->prefix);
      hasrun = true;
      struct IMRequest *rsp = (*cmd)->run(epollfd, client, (char *)command);
      printf("aft run\n");
      if (rsp != NULL) {
        pthread_mutex_lock(&(client->lock));
        send_request(&(client->outbuffer), rsp);
        pthread_mutex_unlock(&(client->lock));
        freeIMRequest(rsp);
      }
      break;
    }
  }
  if (!hasrun) ac_log(AC_LOG_ERROR, "not recognized command");
  printf("aft parse_command\n");
}
