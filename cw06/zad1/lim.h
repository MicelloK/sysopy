// lim.h | plik z nagłówkami dla server i client

#ifndef LIM_H
#define LIM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <sys/msg.h>

#define MAX_CLIENTS 10
#define MAX_MSG_LEN 256

#define MT_INIT 0
#define MT_LIST 1
#define MT_2ALL 2
#define MT_2ONE 3
#define MT_STOP 4

typedef struct msg_buff {
	long mtype;
	int client_id; // idx/pid
	int catcher_pid;
	key_t q_key;
	char content[MAX_MSG_LEN];
} msg_buff;

typedef struct client {
	int id;
	key_t key;
} client;

#endif // LIM_H
