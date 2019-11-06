#ifndef IM_AUTH_H_
#define IM_AUTH_H_

#include <stdbool.h>
#include <stdio.h>

#include "client.h"

#define USERNAME_LIMIT 1024
#define PASSWORD_LIMIT 1024

typedef struct user {
  char *username;
  char *password;
  int last_logged_in;
  int last_active;
  int blocked;
  int attempts;
  struct im_client *client;
} user_t;

typedef struct linked_user {
  user_t *user;
  struct linked_user *next;
} linked_user_t;

// i'm lazy to write hashmap so trie it is
typedef struct trie_user {
  user_t *user;
  struct trie_user *child[256];
} trie_user_t;

typedef struct UserDb {
  linked_user_t *first;
  trie_user_t *root;
  int block_duration;
  int login_timeout;
} UserDb;

UserDb *buildUserDb(FILE *fd, int block_duration, int login_timeout);

user_t *findUser(UserDb *db, const char *username);

int login(UserDb *db, const char *username, const char *password,
          user_t **user);

linked_user_t *loggedInUsers(UserDb *db, bool currentlyOnline);

linked_user_t *loggedInUsersInPastSeconds(UserDb *db, int seconds,
                                          bool currentlyOnline);

linked_user_t *loggedInUsersAfterTime(UserDb *db, int threshold,
                                      bool currentlyOnline);

void freeUserDb(UserDb *db);

#endif
