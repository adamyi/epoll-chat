#define _GNU_SOURCE

#include <memory.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "achelper/ac_memory.h"

#include "client.h"
// do not sort
#include "auth.h"
#include "buffer.h"
#include "file.h"
#include "response.h"
#include "socket.h"

#include "proto/IMResponse.pb.h"
#include "proto/TextResponse.pb.h"

#define MAX_PEERS 256

uint32_t global_chunk_id = 0;

trie_file_t *newTrieFileNode() {
  trie_file_t *node = ac_malloc(sizeof(trie_file_t), "file db trie node");
  memset(node->child, 0, sizeof(trie_file_t *) * 256);
  node->chunks = 0;
  return node;
}

trie_file_t *findFile(trie_file_t *root, const char *filename) {
  trie_file_t *curr = root;
  for (int i = 0; filename[i] != '\0'; i++) {
    if (curr->child[(uint8_t)filename[i]] == NULL) return NULL;
    curr = curr->child[(uint8_t)filename[i]];
  }
  if (curr->chunks > 0) return curr;
  return NULL;
}

trie_file_t *addFile(trie_file_t *root, const char *filename, uint32_t chunks) {
  trie_file_t *curr = root;
  for (int i = 0; filename[i] != '\0'; i++) {
    if (curr->child[(uint8_t)filename[i]] == NULL)
      curr->child[(uint8_t)filename[i]] = newTrieFileNode();
    curr = curr->child[(uint8_t)filename[i]];
  }
  if (curr->chunks > 0) return NULL;  // existing file
  curr->chunk_sid = global_chunk_id;
  curr->chunks = chunks;
  global_chunk_id += chunks;
  return curr;
}

// 0: doesn't have file; 1: has file but not desired chunks; 2: has desired
// chunks
int clientHasFileChunks(im_client_t *client, trie_file_t *file,
                        bool *wantedchunkmask, bool *haschunkmask) {
  int ret = 0;
  uint32_t sid = file->chunk_sid;
  uint32_t eid = sid + file->chunks;
  for (int i = 0; i < client->nchunks; i++) {
    if (client->chunkids[i] >= sid && client->chunkids[i] < eid) {
      haschunkmask[client->chunkids[i] - sid] = true;
      if (ret == 0) ret = 1;
      if (wantedchunkmask[client->chunkids[i] - sid]) ret = 2;
    }
  }
  return ret;
}

void regAllFileChunk(im_client_t *client, trie_file_t *file) {
  client->chunkids = realloc(
      client->chunkids, sizeof(uint32_t) * (client->nchunks + file->chunks));
  for (int i = client->nchunks, j = 0, k = file->chunk_sid; j < file->chunks;
       i++, j++, k++) {
    client->chunkids[i] = k;
  }
  client->nchunks += file->chunks;
}

void regFileChunk(im_client_t *client, trie_file_t *file, uint32_t **chunks) {
  uint32_t count = 0;
  for (; *(chunks + count) != NULL; count++)
    ;
  client->chunkids = realloc(client->chunkids, client->nchunks + count);
  for (int i = client->nchunks, j = 0; j < count; i++, j++) {
    client->chunkids[i] = file->chunk_sid + **(chunks + j);
  }
  client->nchunks += count;
}

void freeFileTrie(trie_file_t *root) {
  for (int i = 0; i < 256; i++) {
    if (root->child[i] != NULL) {
      freeFileTrie(root->child[i]);
    }
  }
  free(root);
}
