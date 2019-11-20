#define _GNU_SOURCE

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "achelper/ac_log.h"
#include "achelper/ac_memory.h"

#include "lib/client.h"

#include "lib/auth.h"
#include "lib/file.h"
#include "lib/socket.h"
#include "proto/IMResponse.pb.h"
#include "proto/SearchFileRequest.pb.h"
#include "proto/TextResponse.pb.h"
#include "regchunk.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"

linked_user_t *usersWithChunks(UserDb *db, trie_file_t *f,
                               bool *wantedchunkmask, bool *haschunkmask,
                               int chfcTarget) {
  linked_user_t *ret = NULL;
  linked_user_t **next = &ret;
  linked_user_t *curr = db->first;
  while (curr != NULL && curr->user != NULL) {
    if (curr->user != NULL && isUserLoggedIn(db, curr->user)) {
      if (clientHasFileChunks(curr->user->client, f, wantedchunkmask,
                              haschunkmask) == chfcTarget) {
        linked_user_t *n = ac_malloc(sizeof(linked_user_t),
                                     "user linked list in usersWithChunks");
        n->user = curr->user;
        n->next = NULL;
        *next = n;
        next = &(n->next);
      }
    }
    curr = curr->next;
  }
  return ret;
}

struct IMResponse *cmd_searchfile_impl(UserDb *db, int epollfd,
                                       im_client_t *client,
                                       struct RegisterChunkRequest *req) {
  struct TextResponse *irsp = malloc(sizeof(struct IMResponse));
  bool success = false;
  if (!isUserLoggedIn(db, client->user)) {
    irsp->msg.len = asprintf(&(irsp->msg.value), "You are not logged in");
  } else {
    trie_file_t *f = findFile(fileDb, (char *)req->filename.value);
    if (f == NULL) {
      irsp->msg.len =
          asprintf(&(irsp->msg.value),
                   "%s is not available on server (not yet registered).",
                   req->filename.value);
    } else {
      size_t n = 0;
      uint32_t **chunks = parse_packed_uint32(&(req->chunks), &n);

      bool *wantedchunkmask = malloc(sizeof(bool) * f->chunks);
      bool *haschunkmask = malloc(sizeof(bool) * f->chunks);
      memset(wantedchunkmask, 0, sizeof(bool) * f->chunks);
      memset(haschunkmask, 0, sizeof(bool) * f->chunks);

      int chfcTarget = 1;
      char *chfcDescription = "any";
      for (uint32_t **curr = chunks; *curr != NULL; curr++) {
        chfcTarget = 2;
        chfcDescription = "your requested";
        if (**curr < f->chunks) wantedchunkmask[**curr] = true;
      }
      free_parsed_packed_uint32(chunks);

      linked_user_t *userlist =
          usersWithChunks(db, f, wantedchunkmask, haschunkmask, chfcTarget);
      char *userlist_str;
      userLinkedListToString(userlist, &userlist_str, "\n");
      freeUserLinkedList(userlist, false);

      uint32_t haschunks = 0;
      for (uint32_t i = 0; i < f->chunks; i++) {
        if (haschunkmask[i]) {
          haschunks++;
        }
      }

      free(wantedchunkmask);
      free(haschunkmask);

      success = true;
      if (haschunks == f->chunks)
        irsp->msg.len = asprintf(
            &(irsp->msg.value),
            "%s is registered on server with %u chunks. All chunks are "
            "currently available. Below are users with %s chunks:\n%s\n"
            "After downloading all chunks, cd into your clientdata directory "
            "and run `cat %s/{0..%u} > %s.merged`",
            req->filename.value, f->chunks, chfcDescription, userlist_str,
            req->filename.value, f->chunks - 1, req->filename.value);
      else
        irsp->msg.len =
            asprintf(&(irsp->msg.value),
                     "%s is registered on server with %u chunks. Only %u "
                     "chunks are available (registered by currently online "
                     "clients). Below are users with %s chunks:\n%s",
                     req->filename.value, f->chunks, haschunks, chfcDescription,
                     userlist_str);
      free(userlist_str);
    }
  }
  return encodeTextResponseToIMResponseAndFree(irsp, success);
}

const im_command_t cmd_searchfile = {.type = 10,
                                     .run = cmd_searchfile_impl,
                                     .parser = parseSearchFileRequestFromBytes,
                                     .freeer = freeSearchFileRequest};
#pragma GCC diagnostic pop
