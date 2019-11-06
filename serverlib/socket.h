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

#define MSG_LIMIT 32768
#define NAME_LIMIT 256

void make_socket_nonblocking(int sockfd);

int listen_socket(int portnum);

struct im_client *im_connection_accept(int epollfd, int sockfd);

void im_receive_command(UserDb *db, struct im_client *client,
                        struct epoll_event *event);

void send_protobuf(struct im_client *client, ac_protobuf_message_t *msg);

#endif
