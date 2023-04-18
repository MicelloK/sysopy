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
#include <poll.h>

#include "lim.h"

int client_idx;
int client_pid;
char qName[LENN];
mqd_t qDesc;
mqd_t sQueueDesc;

int waiting = 0;

int handle_init() {
	time_t msg_time = time(NULL);
	msg_buff msg;
	msg.mtype = INIT;
	msg.client_id = client_pid;
	msg.time_struct = *localtime(&msg_time);

	strcpy(msg.content, qName);
	int sndtest = mq_send(sQueueDesc, (char *) &msg, sizeof(msg_buff), 0);
	int rcvtest = mq_receive(qDesc, (char *) &msg, sizeof(msg_buff), NULL);

	client_idx = msg.client_id;
	if (client_idx == -1 || sndtest == -1 || rcvtest == -1) {
		printf("SERVER | Initialization fault!\n");
	}
	else {
		printf("SERVER | Initialization was successful\n");
	}

	return client_idx;
}

void handle_list() {
	time_t msg_time = time(NULL);
	msg_buff msg;
	msg.mtype = LIST;
	msg.client_id = client_idx;
	msg.time_struct = *localtime(&msg_time);

	mq_send(sQueueDesc, (char *) &msg, sizeof(msg_buff), 0);
	mq_receive(qDesc, (char *) &msg, sizeof(msg_buff), NULL);
	printf(">>> ACTIVE CLIENTS <<<\n");
	printf("%s", msg.content);
}

void handle_2all(char* message) {
	time_t msg_time = time(NULL);
	msg_buff msg;
	msg.mtype = TALL;
	msg.client_id = client_idx;
	msg.time_struct = *localtime(&msg_time);
	strcpy(msg.content, message);

	mq_send(sQueueDesc, (char *) &msg, sizeof(msg_buff), 0);
}

void handle_2one(char* message, int c_pid) {
	time_t msg_time = time(NULL);
	msg_buff msg;
	msg.mtype = TONE;
	msg.client_id = client_idx;
	msg.catcher_pid = c_pid;
	msg.time_struct = *localtime(&msg_time);
	strcpy(msg.content, message);

	mq_send(sQueueDesc, (char *) &msg, sizeof(msg_buff), 0);
}

void handle_stop() {
	time_t msg_time = time(NULL);
	msg_buff msg;
	msg.mtype = STOP;
	msg.client_id = client_idx;
	msg.time_struct = *localtime(&msg_time);

	mq_send(sQueueDesc, (char *) &msg, sizeof(msg_buff), 0);
	mq_close(sQueueDesc);
	exit(EXIT_SUCCESS);
}

void handle_server_message() {
	msg_buff msg_rcv;
	struct timespec my_time;

	clock_gettime(CLOCK_REALTIME, &my_time);
	my_time.tv_sec += 0.1;
	while (mq_timedreceive(qDesc, (char *) &msg_rcv, sizeof(msg_buff), NULL, &my_time) != -1) {
		if (msg_rcv.mtype == STOP) {
			printf("Recived stop message, leaving...\n");
			handle_stop();
		}
		else {
			struct tm msg_time = msg_rcv.time_struct;
			printf("\n[%02d:%02d:%02d] [%d]: \"%s\"\n",
				msg_time.tm_hour,
				msg_time.tm_min,
				msg_time.tm_sec,
	        	msg_rcv.client_id,
	        	msg_rcv.content);
			waiting = 0;
		}
	}
}

void random_name() {
	qName[0] = '/';

	for (int i = 1; i < LENN; i++) {
		qName[i] = random_letter();
	}
}

int main() {
	client_pid = getpid();
	printf("PID: %d\n", client_pid);

	srand(time(NULL));
	random_name();
    qDesc = create_queue(qName);
    sQueueDesc = mq_open(SQUEUE, O_RDWR);
    client_idx = handle_init();

	signal(SIGINT, handle_stop);

	struct pollfd fds[1];
    fds[0].fd = STDIN_FILENO;
    fds[0].events = POLLIN;

	size_t len;
	ssize_t read;
	char* line = NULL;

	printf("AVAIABLE COMMANDS: [LIST|2ALL|2ONE|STOP]\n\n");
	while (1) {
		handle_server_message();

		if (!waiting) {
			printf("> ");
			fflush(stdout);
			waiting = 1;
		}
		int ret = poll(fds, 1, 1000);
		if (ret == 0) {
			continue;
		}
		else if (ret > 0 && fds[0].revents & POLLIN) {
			read = getline(&line, &len, stdin);
			line[read - 1] = '\0';
			waiting = 0;
		}

		if (strcmp(line, "") == 0) {
			continue;
		}

		char* command = strtok(line, " ");

		if (strcmp(command, "INIT") == 0) {
			handle_init();
		}
		else if (client_idx == -1) {
			printf("You need to set server connection first!\n");
		}
		else if (strcmp(command, "LIST") == 0) {
			handle_list();
		}
		else if (strcmp(command, "2ALL") == 0) {
			command = strtok(NULL, " ");
			char* message = command;
			handle_2all(message);
			printf("SERVER | msg was sent\n");
		}
		else if (strcmp(command, "2ONE") == 0) {
			command = strtok(NULL, " ");
			int c_pid = atoi(command);
			command = strtok(NULL, " ");
			char* message = command;
			handle_2one(message, c_pid);
			printf("SERVER | message was sent\n");
		}
		else if (strcmp(command, "STOP") == 0) {
			handle_stop();
		}
		else {
			printf("Unknown command!\n");
		}
	}
}
