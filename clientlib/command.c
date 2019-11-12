#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "achelper/ac_log.h"
#include "achelper/ac_protobuf.h"

#include "client.h"
// do not sort
#include "auth.h"

#include "proto/IMRequest.pb.h"
#include "proto/IMResponse.pb.h"

#include "clientlib/commands/block.h"
#include "clientlib/commands/broadcast.h"
#include "clientlib/commands/login.h"
#include "clientlib/commands/message.h"
#include "clientlib/commands/unblock.h"

#include "clientlib/socket.h"

const im_command_t *enabled_commands[] = {
    &cmd_login, &cmd_message, &cmd_broadcast, &cmd_block, &cmd_unblock, NULL};

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

size_t parse_response(int epollfd, im_client_t *client, uint8_t *cmd,
                      size_t len, struct IMResponse **rsp) {
  size_t ret = 0;
  ac_protobuf_message_t *msg =
      ac_decode_protobuf_msg_with_n_fields(cmd, len, 2, &ret);
  if (msg == NULL) {
    ac_log(AC_LOG_ERROR, "protobuf decode failure: invalid protobuf");
    return 0;
  }
  struct IMResponse *imrsp = parseIMResponseFromProtobufMsg(msg);

  ac_protobuf_print_msg(msg);
  printf("%s\n", imrsp->msg.value);
  freeIMResponse(imrsp);
  return ret;
}

void parse_command(int epollfd, im_client_t *client, uint8_t *command,
                   size_t len, struct IMResponse **rsp) {
  bool hasrun = false;
  for (const im_command_t **cmd = enabled_commands; *cmd != NULL; cmd++) {
    printf("checking %p\n", cmd);
    if (strlen((*cmd)->prefix) <= len &&
        strncmp((*cmd)->prefix, command, strlen((*cmd)->prefix)) == 0) {
      ac_log(AC_LOG_DEBUG, "found implementation for command %s",
             (*cmd)->prefix);
      hasrun = true;
      struct IMRequest *rsp = (*cmd)->run(epollfd, client, command);
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
}
