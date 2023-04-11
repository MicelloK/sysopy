#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>

#include "lim.h"

int server_q;
client clients[MAX_CLIENTS];

void signal_handler(int sig) {
	if (server_q != -1) {
		msg_buff *msg = malloc(sizeof(msg_buff));
		msg->mtype = MT_STOP;
		for (int i = 0; i < MAX_CLIENTS; i++) {
			if (clients[i].key != -1) {
				int client_qid = msgget(clients[i].key, 0);
				msgsnd(client_qid, msg, sizeof(msg_buff), 0);
			}
		}
	}

	msgctl(server_q, IPC_RMID, NULL);
	exit(EXIT_SUCCESS);
}

void rcv_init(msg_buff *msg) {
	printf("awdsf");
	int client_idx = 0; 
	while (client_idx < MAX_CLIENTS && clients[client_idx].key == -1) {
		client_idx++;
	}

	if (client_idx == MAX_CLIENTS) {
		printf("SERVER | Server can only handle %d clients!\n", MAX_CLIENTS);
		msg->client_id = -1;
	}
	else {
		clients[client_idx].id = msg->client_id; // pid
		clients[client_idx].key = msg->q_key;
	}

	msg->client_id = client_idx;
	int client_qid = msgget(clients[client_idx].key, 0);
	msgsnd(client_qid, msg, sizeof(msg_buff), 0);

	printf("SERVER | New client: %d\n", clients[client_idx].id);
}

void rcv_list(msg_buff *msg) {
	int idx = msg->client_id;
	strcpy(msg->content, "ACTIVE CLIENTS:\n"); 

	for (int i = 0; i < MAX_CLIENTS; i++) {
		if (clients[i].key != -1 && i != idx) {
			char *tmp = malloc(MAX_MSG_LEN);
			sprintf(tmp, "ID: %d\n", clients[i].id);
			strcat(msg->content, tmp);
			free(tmp);
		}
	}

	int client_qid = msgget(clients[idx].key, 0);
	msgsnd(client_qid, msg, sizeof(msg_buff), 0);
}

void rcv_2all(msg_buff *msg) {
	int idx = msg->client_id;
	for (int i = 0; i < MAX_CLIENTS; i++) {
		if (i != idx) {
			int client_qid = msgget(clients[idx].key, 0);
			msgsnd(client_qid, msg, sizeof(msg_buff), 0);
		}
	}
}

void rcv_2one(msg_buff *msg) {
	int catcher_pid = msg->catcher_pid;

	int i = 0;
	while(i < MAX_CLIENTS && clients[i].id != catcher_pid) {
		i++;
	}
	if (i == MAX_CLIENTS) {
		printf("Can not find [%d] user\n", catcher_pid);
	}

	int client_qid = msgget(clients[i].key, 0);
	msgsnd(client_qid, msg, sizeof(msg_buff), 0);
}

void rcv_stop(msg_buff *msg) {
	int idx = msg->client_id;
	int client_qid = msgget(clients[idx].key, 0);
	clients[idx].id = -1;
	clients[idx].key = -1;
	msgsnd(client_qid, msg, sizeof(msg_buff), 0);
}

int main() {
	signal(SIGINT, signal_handler);

	for (int i = 0; i < MAX_CLIENTS; i++) {
		client *new_client = malloc(sizeof(client));
		new_client->key = -1;
		clients[i] = *new_client;
	}

	// open server q
	key_t s_key = ftok(getenv("HOME"), 1);
	if (s_key == -1) {
		perror("ftok");
		exit(EXIT_FAILURE);
	}

	server_q = msgget(s_key, IPC_CREAT | 0666);
	if (server_q == -1) {
		perror("msgget");
		exit(EXIT_FAILURE);
	}

	printf("SERVER | starting server...\n");

	msg_buff *msg = malloc(sizeof(msg_buff));
	while (1) {
		msgrcv(server_q, msg, sizeof(msg_buff) - sizeof(long), 0, 0);

		int type = msg->mtype;

		switch(type) {
			case MT_INIT:
				rcv_init(msg);
				break;
			case MT_STOP:
				rcv_stop(msg);
				break;
			case MT_LIST:
				rcv_list(msg);
				break;
			case MT_2ALL:
				rcv_2all(msg);
				break;
			case MT_2ONE:
				rcv_2one(msg);
				break;
			default:
				printf("Wrong msg type [%ld]\n", msg->mtype);
				type = -1;
		}
	}
}