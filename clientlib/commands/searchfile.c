#define _GNU_SOURCE

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "achelper/ac_log.h"

#include "proto/IMRequest.pb.h"
#include "proto/SearchFileRequest.pb.h"
#include "searchfile.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"

struct IMRequest *cmd_searchfile_impl(int epollfd, im_client_t *client,
                                      char *req) {
  struct IMRequest *ret = malloc(sizeof(struct IMRequest));
  ret->type = 10;
  char *filename = req + 11;

  struct SearchFileRequest *mreq = malloc(sizeof(struct SearchFileRequest));
  mreq->filename.len = asprintf(&(mreq->filename.value), "%s", filename);
  mreq->chunks.len = 0;
  ret->value.value = encodeSearchFileRequestToBytes(mreq, &(ret->value.len));
  freeSearchFileRequest(mreq);
  return ret;
}

const im_command_t cmd_searchfile = {.prefix = "searchFile ",
                                     .run = cmd_searchfile_impl};
#pragma GCC diagnostic pop
