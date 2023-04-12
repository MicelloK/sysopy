#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <time.h>
#include <errno.h>

#include "lim.h"

key_t key;
int client_idx;
int client_pid;
int server_q;
int client_qid;

void handle_init() {
	msg_buff *msg = malloc(sizeof(msg_buff));
	msg->mtype = INIT;
	msg->q_key = key;
	msg->client_id = client_pid;

	int sndtest = msgsnd(server_q, msg, sizeof(msg_buff), 0);
	int rcvtest = msgrcv(client_qid, msg, sizeof(msg_buff), 0, 0);

	client_idx = msg->client_id;
	if (client_idx == -1 || sndtest == -1 || rcvtest == -1) {
		printf("SERVER | Initialization fault!\n");
	}
	else {
		printf("SERVER | Initialization was successful\n");
	}
}

void handle_list() {
	msg_buff *msg = malloc(sizeof(msg_buff));
	msg->mtype = LIST;
	msg->client_id = client_idx;

	msgsnd(server_q, msg, sizeof(msg_buff), 0);
	msgrcv(client_qid, msg, sizeof(msg_buff), 0, 0);
	printf(">>> SERVER MSG <<<\n");
	printf("%s", msg->content);
}

void handle_2all(char* message) {
	msg_buff *msg = malloc(sizeof(msg_buff));
	msg->mtype = TALL;
	msg->client_id = client_idx;
	strcpy(msg->content, message);

	msgsnd(server_q, msg, sizeof(msg_buff), 0);
}

void handle_2one(char* message, int c_pid) {
	msg_buff *msg = malloc(sizeof(msg_buff));
	msg->mtype = TONE;
	msg->client_id = client_idx;
	msg->catcher_pid = c_pid;
	strcpy(msg->content, message);

	msgsnd(server_q, msg, sizeof(msg_buff), 0);
}

void handle_stop() {
	msg_buff *msg = malloc(sizeof(msg_buff));
	msg->mtype = STOP;
	msg->client_id = client_idx;

	msgsnd(server_q, msg, sizeof(msg_buff), 0);
	msgctl(client_qid, IPC_RMID, NULL);
	exit(EXIT_SUCCESS);
}

int main() {
	srand(time(NULL));
	key = ftok(getenv("HOME"), rand() % 255 + 1);
	client_qid = msgget(key, IPC_CREAT | 0666);
	if (client_qid == -1) {
		perror("msgget");
		exit(1);
	}
	key_t server_key = ftok(getenv("HOME"), 1);
	server_q = msgget(server_key, 0);
	client_pid = getpid();
	handle_init();

	signal(SIGINT, handle_stop);

	size_t len;
	ssize_t read;
	char* command = NULL;

	while (1) {
		printf("AVAIABLE COMMANDS: [LIST|2ALL|2ONE|STOP]\n");
		printf("> ");

		read = getline(&command, &len, stdin);
		command[read - 1] = '\0';

		if (strcmp(command, "") == 0) {
			continue;
		}
		else if (strcmp(command, "INIT") == 0) {
			handle_init();
		}
		else if (client_idx == -1) {
			printf("You need to set server connection first!\n");
		}
		else if (strcmp(command, "LIST") == 0) {
			handle_list();
		}
		else if (strcmp(command, "2ALL") == 0) {
			char* message;
			size_t msg_len;
			ssize_t msg_read;

			printf("Enter your message: ");
			msg_read = getline(&message, &msg_len, stdin);
			message[msg_read - 1] = '\0';
			handle_2all(message);
			printf("SERVER | msg was sent\n");
		}
		else if (strcmp(command, "2ONE") == 0) {
			char* message;
			size_t msg_len;
			ssize_t msg_read;
			int c_pid;

			printf("Enter recipient pid: ");
			scanf("%d", &c_pid);

			printf("Enter your message: ");
			msg_read = getline(&message, &msg_len, stdin);
			message[msg_read - 1] = '\0';
			handle_2one(message, c_pid);
			printf("SERVER | msg was sent\n");
		}
		else if (strcmp(command, "STOP") == 0) {
			handle_stop();
		}
		else {
			printf("Unknown command!\n");
		}
	}
}
