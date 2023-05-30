#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>
#include <pthread.h>
#include <poll.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#define MSG_SIZE 1024
#define CL_NAME_LEN 16

#define SOCKET_PROTOCOL SOCK_STREAM
#define SERVER_NAME "server"
#define SERVER_CLIENT_LIMIT 10
#define PING_INTERVAL 5
#define PING_TIMEOUT 5

#define REGISTER 0
#define PING 1
#define LIST 2
#define TALL 3
#define TONE 4
#define STOP 5

#define UNIX_MODE 0
#define INET_MODE 1

struct client {
	int id;
	char name[CL_NAME_LEN + 1];
	int socket_fd;
	int registered;
	int active;
};

struct msg {
	int type;
	char msg[MSG_SIZE];
};

#endif