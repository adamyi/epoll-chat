#include <memory.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "achelper/ac_memory.h"

#include "client.h"
// do not sort
#include "auth.h"
#include "buffer.h"
#include "proto/IMResponse.pb.h"

trie_user_t *newTrieUserNode() {
  trie_user_t *node = ac_malloc(sizeof(trie_user_t), "user db trie node");
  memset(node->child, 0, sizeof(trie_user_t *) * 256);
  node->user = NULL;
  return node;
}

UserDb *buildUserDb(FILE *fd, int block_duration, int login_timeout) {
  UserDb *db = ac_malloc(sizeof(UserDb), "user db");
  db->root = newTrieUserNode();
  db->block_duration = block_duration;
  db->login_timeout = login_timeout;
  char username[USERNAME_LIMIT];
  char password[USERNAME_LIMIT];
  linked_user_t **next = &(db->first);
  while (fscanf(fd, "%s %s", username, password) != EOF) {
    user_t *newuser = ac_malloc(sizeof(user_t), "new user");
    newuser->username = strdup(username);
    newuser->password = strdup(password);
    newuser->last_logged_in = -1;
    newuser->last_active = -1;
    newuser->last_blocked = -1;
    newuser->attempts = 0;
    newuser->client = NULL;
    newuser->blocked = NULL;
    init_buffer(&(newuser->buffer), OUT_BUFFER_DEFAULT_SIZE);

    linked_user_t *newuser_link =
        ac_malloc(sizeof(linked_user_t), "user user linked list");
    newuser_link->user = newuser;
    newuser_link->next = NULL;
    *next = newuser_link;
    next = &(newuser_link->next);

    trie_user_t *curr = db->root;
    for (int i = 0; username[i] != '\0'; i++) {
      if (curr->child[(uint8_t)username[i]] == NULL)
        curr->child[(uint8_t)username[i]] = newTrieUserNode();
      curr = curr->child[(uint8_t)username[i]];
    }
    curr->user = newuser;
  }
  return db;
}

user_t *findUser(UserDb *db, const char *username) {
  trie_user_t *curr = db->root;
  for (int i = 0; username[i] != '\0'; i++) {
    if (curr->child[(uint8_t)username[i]] == NULL) return NULL;
    curr = curr->child[(uint8_t)username[i]];
  }
  return curr->user;
}

// 0: success
// 1: user not found
// 2: user blocked
// 3: wrong password
// 4: another session is in place
int login(UserDb *db, int epollfd, const char *username, const char *password,
          user_t **user) {
  int ct = (int)time(NULL);
  printf("%s\n", username);
  printf("%s\n", password);
  *user = findUser(db, username);
  printf("%p %p\n", user, *user);
  if (*user == NULL) return 1;
  if (ct < (*user)->last_blocked) return 2;
  if (strcmp((*user)->password, password) != 0) {
    if ((++((*user)->attempts)) == 3) {
      (*user)->last_blocked = ct + db->block_duration;
      (*user)->attempts = 0;
      return 2;
    }
    return 3;
  }
  if (isUserLoggedIn(db, *user)) return 4;
  (*user)->attempts = 0;
  (*user)->last_active = (*user)->last_logged_in = (int)time(NULL);

  // send login message to other online users
  struct IMResponse *rsp = malloc(sizeof(struct IMResponse));
  rsp->success = true;
  rsp->msg.len = asprintf(&(rsp->msg.value), "%s has logged in", username);
  linked_user_t *curr = db->first;
  while (curr != NULL) {
    if (curr->user != NULL && curr->user != *user &&
        isUserLoggedIn(db, curr->user) &&
        (!hasBlockedUser(*user, curr->user))) {
      send_response_to_user(db, epollfd, curr->user, rsp);
    }
    curr = curr->next;
  }
  freeIMResponse(rsp);

  return 0;
}

bool hasBlockedUser(user_t *by, user_t *u) {
  linked_user_t *curr = by->blocked;
  while (curr != NULL) {
    if (curr->user == u) return true;
    curr = curr->next;
  }
  return false;
}

void blockUser(user_t *by, user_t *u) {
  if (u == by) return;  // cannot block self
  if (hasBlockedUser(by, u)) return;
  linked_user_t *newblock = malloc(sizeof(linked_user_t));
  newblock->user = u;
  newblock->next = by->blocked;
  by->blocked = newblock;
}

void unblockUser(user_t *by, user_t *u) {
  linked_user_t **curr = &(by->blocked);
  while (*curr != NULL) {
    if ((*curr)->user == u) {
      *curr = (*curr)->next;
    }
    if (*curr != NULL) curr = &((*curr)->next);
  }
}

linked_user_t *loggedInUsers(UserDb *db, bool currentlyOnline,
                             user_t *exclude) {
  return loggedInUsersAfterTime(db, 0, currentlyOnline, exclude);
}

linked_user_t *loggedInUsersInPastSeconds(UserDb *db, int seconds,
                                          bool currentlyOnline,
                                          user_t *exclude) {
  int threshold = (int)time(NULL) - seconds;
  return loggedInUsersAfterTime(db, threshold, currentlyOnline, exclude);
}

linked_user_t *loggedInUsersAfterTime(UserDb *db, int threshold,
                                      bool currentlyOnline, user_t *exclude) {
  linked_user_t *ret = NULL;
  linked_user_t **next = &ret;
  linked_user_t *curr = db->first;
  while (curr != NULL && curr->user != NULL) {
    if ((curr->user != exclude) && (curr->user->last_active >= threshold) &&
        ((!currentlyOnline) || isUserLoggedIn(db, curr->user))) {
      linked_user_t *n =
          ac_malloc(sizeof(linked_user_t), "user linked list in loggedInUsers");
      n->user = curr->user;
      n->next = NULL;
      *next = n;
      next = &(n->next);
    }
    curr = curr->next;
  }
  return ret;
}

bool isUserLoggedIn(UserDb *db, user_t *user) {
  if (user == NULL) return false;
  if (user->client == NULL) return false;
  int ct = (int)time(NULL);
  return user->last_active + db->login_timeout > ct;
}

void logoutUser(UserDb *db, int epollfd, user_t *user) {
  if (user == NULL) return;
  if (user->client == NULL) return;
  user->client->user = NULL;
  user->client = NULL;

  // send logout message to other online users
  struct IMResponse *rsp = malloc(sizeof(struct IMResponse));
  rsp->success = true;
  rsp->msg.len =
      asprintf(&(rsp->msg.value), "%s has logged out", user->username);
  linked_user_t *curr = db->first;
  while (curr != NULL) {
    if (curr->user != NULL && curr->user != user &&
        isUserLoggedIn(db, curr->user) && (!hasBlockedUser(user, curr->user))) {
      send_response_to_user(db, epollfd, curr->user, rsp);
    }
    curr = curr->next;
  }
  freeIMResponse(rsp);
}

size_t userLinkedListToString(linked_user_t *list, char **str) {
  linked_user_t *curr = list;
  if (curr == NULL) {
    *str = NULL;
    return 0;
  }
  size_t ret = 0;
  while (curr != NULL) {
    if (curr->user != NULL) {
      ret += 2 + strlen(curr->user->username);
    }
    curr = curr->next;
  }
  ret -= 2;
  *str = malloc(ret + 1);
  char *nxt = *str;
  curr = list;
  while (curr != NULL) {
    if (curr->user != NULL) {
      if (nxt != *str) {
        *(nxt++) = ',';
        *(nxt++) = ' ';
      }
      size_t len = strlen(curr->user->username);
      memcpy(nxt, curr->user->username, len);
      nxt += len;
    }
    curr = curr->next;
  }
  *nxt = '\0';
  return ret;
}

void freeUserTrie(trie_user_t *root) {
  for (int i = 0; i < 256; i++) {
    if (root->child[i] != NULL) {
      freeUserTrie(root->child[i]);
    }
  }
  free(root);
}

void freeUserLinkedList(linked_user_t *first, bool freeUser) {
  linked_user_t *curr = first;
  linked_user_t *next = first;
  if (curr != NULL) {
    while (curr != NULL) {
      next = curr->next;
      if (freeUser && curr->user != NULL) {
        free(curr->user->username);
        free(curr->user->password);
        free(curr->user);
      }
      curr = next;
    }
  }
}

void freeUserDb(UserDb *db) {
  freeUserLinkedList(db->first, true);
  freeUserTrie(db->root);
  free(db);
}
