#ifndef IM_AUTH_H_
#define IM_AUTH_H_

#include <stdbool.h>
#include <stdio.h>

#include "buffer.h"
#include "client.h"

#define USERNAME_LIMIT 1024
#define PASSWORD_LIMIT 1024

extern bool IS_SERVER;

typedef struct user {
  char *username;
  char *password;
  int last_logged_in;
  int last_active;
  int last_blocked;
  int attempts;
  struct im_client *client;
  im_buffer_t buffer;
  struct linked_user *blocked;
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

UserDb *newUserDb(int block_duration, int login_timeout);

UserDb *buildUserDb(FILE *fd, int block_duration, int login_timeout);

user_t *addUser(UserDb *db, const char *username, const char *password);

user_t *findUser(UserDb *db, const char *username);

user_t *findOrAddUser(UserDb *db, const char *username);

int login(UserDb *db, int epollfd, const char *username, const char *password,
          user_t **user);

bool hasBlockedUser(user_t *by, user_t *u);

void blockUser(user_t *by, user_t *u);

void unblockUser(user_t *by, user_t *u);

linked_user_t *loggedInUsers(UserDb *db, bool currentlyOnline, user_t *exclude);

linked_user_t *loggedInUsersInPastSeconds(UserDb *db, int seconds,
                                          bool currentlyOnline,
                                          user_t *exclude);

linked_user_t *loggedInUsersAfterTime(UserDb *db, int threshold,
                                      bool currentlyOnline, user_t *exclude);

size_t userLinkedListToString(linked_user_t *list, char **str,
                              const char *separator);

bool isUserLoggedIn(UserDb *db, user_t *user);

void logoutUser(UserDb *db, int epollfd, user_t *user);

void freeUserTrie(trie_user_t *root);

void freeUserLinkedList(linked_user_t *first, bool freeUser);

void freeUserDb(UserDb *db);

#endif
