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

trie_user_t *newTrieUserNode() {
  trie_user_t *node = ac_malloc(sizeof(trie_user_t), "user db trie node");
  memset(node->child, 0, sizeof(trie_user_t *) * 256);
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
    newuser->blocked = -1;
    newuser->attempts = 0;
    newuser->client = NULL;
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
int login(UserDb *db, const char *username, const char *password,
          user_t **user) {
  int ct = (int)time(NULL);
  *user = findUser(db, username);
  if (*user == NULL) return 1;
  if (ct < (*user)->blocked) return 2;
  if (strcmp((*user)->password, password) != 0) {
    if ((++((*user)->attempts)) == 3) {
      (*user)->blocked = ct + db->block_duration;
      (*user)->attempts = 0;
      return 2;
    }
    return 3;
  }
  if (isUserLoggedIn(db, *user)) return 4;
  (*user)->attempts = 0;
  (*user)->last_logged_in = (int)time(NULL);
  return 0;
}

linked_user_t *loggedInUsers(UserDb *db, bool currentlyOnline) {
  return loggedInUsersAfterTime(db, 0, currentlyOnline);
}

linked_user_t *loggedInUsersInPastSeconds(UserDb *db, int seconds,
                                          bool currentlyOnline) {
  int threshold = (int)time(NULL) - seconds;
  return loggedInUsersAfterTime(db, threshold, currentlyOnline);
}

linked_user_t *loggedInUsersAfterTime(UserDb *db, int threshold,
                                      bool currentlyOnline) {
  int ct = (int)time(NULL);
  linked_user_t *ret;
  linked_user_t **next = &ret;
  linked_user_t *curr = db->first;
  while (curr != NULL && curr->user != NULL) {
    if ((curr->user->last_logged_in > threshold) &&
        ((!currentlyOnline) ||
         (curr->user->last_active + db->login_timeout > ct))) {
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
  int ct = (int)time(NULL);
  return user->last_active + db->login_timeout > ct;
}

void freeUserTrie(trie_user_t *root) {
  for (int i = 0; i < 256; i++) {
    if (root->child[i] != NULL) {
      freeUserTrie(root->child[i]);
    }
  }
  free(root);
}

void freeUserDb(UserDb *db) {
  linked_user_t *curr = db->first;
  if (curr != NULL) {
    linked_user_t *next = curr->next;
    while (curr != NULL) {
      if (curr->user != NULL) {
        free(curr->user->username);
        free(curr->user->password);
        free(curr->user);
      }
      free(curr);
      curr = next;
      next = next->next;
    }
  }
  freeUserTrie(db->root);
  free(db);
}
