#ifndef IM_SOCKET_H_
#define IM_SOCKET_H_

#include <fcntl.h>
#include <netinet/in.h>
#include <poll.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "client.h"
// do not sort
#include "auth.h"

#include "proto/IMResponse.pb.h"

#define MSG_LIMIT 32768
#define NAME_LIMIT 256

void make_socket_nonblocking(int sockfd);

int listen_socket(int portnum);

im_client_t *im_connection_accept(int epollfd, int sockfd,
                                  struct sockaddr_in clientaddr);

void im_receive_command(int epollfd, UserDb *db, im_client_t *client,
                        struct epoll_event *event,
                        size_t (*handler)(UserDb *db, int epollfd,
                                          im_client_t *client, uint8_t *cmd,
                                          size_t len, struct IMResponse **rsp));

void im_send_buffer(int epollfd, UserDb *db, im_client_t *client,
                    im_buffer_t *buffer);

void send_response(im_buffer_t *buffer, struct IMResponse *msg);

void send_response_to_client(int epollfd, im_client_t *client,
                             struct IMResponse *msg);

void send_response_to_user(UserDb *db, int epollfd, user_t *user,
                           struct IMResponse *msg);

void close_socket(int epollfd, UserDb *db, im_client_t *client);

int pick_client(struct im_client *clients[], int sockfd, int *nclients,
                bool createNew);

#endif
