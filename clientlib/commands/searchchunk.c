#define _GNU_SOURCE

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "achelper/ac_log.h"

#include "proto/IMRequest.pb.h"
#include "proto/SearchFileRequest.pb.h"
#include "searchchunk.h"

#include "lib/socket.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"

struct IMRequest *cmd_searchchunk_impl(int epollfd, im_client_t *client,
                                       char *req) {
  char *filename = req + 12;

  char *chunkseq = filename;

  while (*chunkseq != ' ') {
    if (*chunkseq == '\0') {
      printf(
          "Error: invalid command. Please specify chunks or use searchFile\n");
      return NULL;
    }
    chunkseq++;
  }
  *(chunkseq++) = '\0';
  struct SearchFileRequest *mreq = malloc(sizeof(struct SearchFileRequest));
  mreq->filename.len = asprintf(&(mreq->filename.value), "%s", filename);
  mreq->chunks.len = 0;
  printf("chunkseq: %s\n", chunkseq);
  size_t nchunks = pack_repeated_uint32_from_str(chunkseq, &(mreq->chunks));
  if (nchunks == 0) {
    printf(
        "Error: invalid command. Please specify chunks correctly or use "
        "searchFile\n");
    freeSearchFileRequest(mreq);
    return NULL;
  }

  struct IMRequest *ret = malloc(sizeof(struct IMRequest));
  ret->type = 10;
  ret->value.value = encodeSearchFileRequestToBytes(mreq, &(ret->value.len));
  freeSearchFileRequest(mreq);
  return ret;
}

const im_command_t cmd_searchchunk = {.prefix = "searchChunk ",
                                      .run = cmd_searchchunk_impl};
#pragma GCC diagnostic pop
