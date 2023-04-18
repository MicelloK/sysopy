#ifndef LIM_H
#define LIM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <mqueue.h>

#define LENN 3
#define SQUEUE "/SERVER"
#define MAX_CLIENTS 10
#define MAX_MSG_LEN 256

typedef struct msg_buff {
	long mtype;
	int client_id; // idx/pid
	int catcher_pid;
	char content[MAX_MSG_LEN];
	struct tm time_struct;
} msg_buff;

typedef enum mtype {
	INIT = 1,
	LIST = 2,
	TALL = 3,
	TONE = 4,
	STOP = 5
} mtype;

char random_letter() {
	return rand() % ('Z' - 'A' + 1) + 'A';
}

typedef struct client {
	int id;
	char * content;
} client;

mqd_t create_queue(const char* name) {
	struct mq_attr attr;
	attr.mq_maxmsg = MAX_CLIENTS;
	attr.mq_msgsize = sizeof(msg_buff);
	return mq_open(name, O_RDWR | O_CREAT, 0666, &attr);
}

#endif // LIM_H