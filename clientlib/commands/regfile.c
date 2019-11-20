#define _GNU_SOURCE

#include <linux/limits.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "achelper/ac_log.h"

#include "proto/IMRequest.pb.h"
#include "proto/RegisterFileRequest.pb.h"
#include "regfile.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"

#define REGFILE_BUFFER_SIZE BUFSIZ
struct IMRequest *cmd_regfile_impl(int epollfd, im_client_t *client,
                                   char *req) {
  char inbuffer[REGFILE_BUFFER_SIZE];
  char filepath[REGFILE_BUFFER_SIZE];
  while (true) {
    printf(
        "You're trying to register a new file on our P2P network.\nIf you are "
        "trying to register only a (or few) chunks of a file, don't use this "
        "command. After you download a chunk from another peer, it is "
        "automatically registered on the server.\nThis is for new files "
        "only.\nAre you sure this is what you want to do? [y/n]: ");
    fgets(inbuffer, REGFILE_BUFFER_SIZE, stdin);
    if (inbuffer[0] == 'y' || inbuffer[0] == 'Y') break;
    if (inbuffer[0] == 'n' || inbuffer[0] == 'N') return NULL;
    printf("I didn't get that...\n\n");
  }
  struct stat fs;
  long bytes = 0;
  char *filename;
  while (true) {
    printf("Path to your local file: ");
    fgets(filepath, REGFILE_BUFFER_SIZE, stdin);
    filepath[strlen(filepath) - 1] = '\0';
    if (stat(filepath, &fs) != 0) {
      perror("Couldn't get file info... Are you sure your path is correct?");
      continue;
    }
    if (!S_ISREG(fs.st_mode)) {
      printf(
          "That's not a regular file (could be a directory, etc)... Choose "
          "another file.\n");
      continue;
    }
    bytes = (long)fs.st_size;
    if (bytes == 0) {
      printf("That file is empty (0 bytes)... Please choose another file.\n");
      continue;
    }
    int stpos;
    for (stpos = strlen(filepath) - 1; stpos >= 0 && filepath[stpos] != '/';
         stpos--)
      ;
    filename = filepath + stpos + 1;
    break;
  }
  printf("%s is a valid file with %ld bytes!\n", filename, bytes);
  long chunksize = 1024;
  printf("How large do you want a chunk to be (in bytes) [default 1024]: ");
  fgets(inbuffer, REGFILE_BUFFER_SIZE, stdin);
  char *endptr;
  errno = 0;
  chunksize = strtol(inbuffer, &endptr, 10);
  if (errno != 0 || chunksize == 0) {
    chunksize = 1024;
  }

  long nchunks = bytes / chunksize;
  if (nchunks * chunksize < bytes) nchunks++;

  while (true) {
    printf("\n\n");
    printf("===P2P FILE REGISTRATION===\n");
    printf("File name: %s\n", filename);
    printf("Local file path: %s\n", filepath);
    printf("File size: %ld bytes\n", bytes);
    printf("Chunk size: %ld bytes\n", chunksize);
    printf("Total Chunks: %ld\n", nchunks);
    printf("\nCan you confirm? [y/n]: ");
    fgets(inbuffer, REGFILE_BUFFER_SIZE, stdin);
    if (inbuffer[0] == 'y' || inbuffer[0] == 'Y') break;
    if (inbuffer[0] == 'n' || inbuffer[0] == 'N') return NULL;
    printf("I didn't get that...\n");
  }

  char *split_filepath = malloc(PATH_MAX);
  sprintf(split_filepath, "%s/%s", datapath, filename);
  if (mkdir(split_filepath, 0700) < 0) {
    printf(
        "Error: couldn't create %s. Did you already register file of same "
        "name?",
        split_filepath);
    free(split_filepath);
    return NULL;
  }
  char *buf = malloc(chunksize);
  FILE *fp = fopen(filepath, "rb");
  for (long i = 0; i < nchunks; i++) {
    sprintf(split_filepath, "%s/%s/%ld", datapath, filename, i);
    FILE *sfp = fopen(split_filepath, "wb");
    size_t cs = fread(buf, 1, chunksize, fp);
    fwrite(buf, 1, cs, sfp);
    fclose(sfp);
  }
  fclose(fp);

  struct IMRequest *ret = malloc(sizeof(struct IMRequest));
  ret->type = 8;
  struct RegisterFileRequest *lreq = malloc(sizeof(struct RegisterFileRequest));
  lreq->filename.len = asprintf(&(lreq->filename.value), "%s", filename);
  lreq->chunks = nchunks;
  ret->value.value = encodeRegisterFileRequestToBytes(lreq, &(ret->value.len));
  freeRegisterFileRequest(lreq);
  return ret;
}

const im_command_t cmd_regfile = {.prefix = "register",
                                  .run = cmd_regfile_impl};
#pragma GCC diagnostic pop
