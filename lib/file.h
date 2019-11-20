#ifndef IM_FILE_H_
#define IM_FILE_H_

#include <stdbool.h>
#include <stdio.h>

#include "buffer.h"
#include "client.h"

#define USERNAME_LIMIT 1024
#define PASSWORD_LIMIT 1024

typedef struct trie_file {
  struct trie_file *child[256];
  uint32_t chunks;
  uint32_t chunk_sid;
} trie_file_t;

trie_file_t *newTrieFileNode();

trie_file_t *addFile(trie_file_t *root, const char *filename, uint32_t chunks);

// 0: doesn't have file; 1: has file but not desired chunks; 2: has desired
// chunks
int clientHasFileChunks(im_client_t *client, trie_file_t *file,
                        bool *wantedchunkmask, bool *haschunkmask);

void regAllFileChunk(im_client_t *client, trie_file_t *file);

trie_file_t *findFile(trie_file_t *root, const char *filename);

void regFileChunk(im_client_t *client, trie_file_t *file, uint32_t **chunks);

void freeFileTrie(trie_file_t *root);

#endif
