#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <signal.h>

#define CHAIR_SEM_ID 0
#define WAITING_SEM_ID 1
#define TIMES_SEM_ID 2

int M, N, P, F;
int *service_times; // ns
int semDesc;
int shmDesc;
int run = 1;
key_t key;
struct sembuf sem_buf;
char time_msg[16];

void set_time() {
	struct tm time_struct;
	time_t my_time = time(NULL);
	time_struct = *localtime(&my_time);
	sprintf(time_msg, "[%02d:%02d:%02d]", time_struct.tm_hour, time_struct.tm_min, time_struct.tm_sec);
}

void signal_handler(int signum) {
	run = 0;
}

void hairdresser_sig_handler(int signum) {
	run = 0;
	exit(EXIT_SUCCESS);
}

void client_loop() {
	int last_client = 0;
	int *times;
	while(run) {
		sleep(rand() % 4);
		int service_number = rand()%F;
		int time = service_times[service_number];
		set_time();
		printf("%s New client has arrived. Service number: %d, time: %fs\n", time_msg, service_number+1, time/1000.0);

		sem_buf.sem_num = TIMES_SEM_ID;
		sem_buf.sem_op = -1;
		semop(semDesc, &sem_buf, 1);

		times = shmat(shmDesc, NULL, 0);
		if (times[last_client+1] == -1) {
			times[last_client+1] = time;
			last_client = (last_client + 1) % P;

			sem_buf.sem_num = WAITING_SEM_ID;
			sem_buf.sem_op = 1;
			semop(semDesc, &sem_buf, 1);
			set_time();
			printf("%s Client take place in the queue\n", time_msg);
		}
		else {
			set_time();
			printf("%s No empty place in queue, leaving..\n", time_msg);
		}
		shmdt(times);
		sem_buf.sem_num = TIMES_SEM_ID;
		sem_buf.sem_op = 1;
		semop(semDesc, &sem_buf, 1);

	}
}

void hairdresser_loop(int id) {
	int *times;
	set_time();
	printf("%s Hairdresser [%d] started working...\n", time_msg, id);
	while(run) {
		sem_buf.sem_num = CHAIR_SEM_ID;
		sem_buf.sem_op = -1;
		semop(semDesc, &sem_buf, 1);

		sem_buf.sem_num = WAITING_SEM_ID;
		sem_buf.sem_op = -1;
		semop(semDesc, &sem_buf, 1);
		set_time();
		printf("%s Hairdresser [%d] has reserved chair, taking client..\n", time_msg, id);

		times = shmat(shmDesc, NULL, 0);
		sem_buf.sem_num = TIMES_SEM_ID;
		sem_buf.sem_op = -1;
		semop(semDesc, &sem_buf, 1);

		int time = times[times[0]+1];
		times[times[0]+1] = -1;
		times[0] = (times[0] + 1) % P;
		shmdt(times);
		sem_buf.sem_op = 1;
		semop(semDesc, &sem_buf, 1);

		usleep(time);
		set_time();
		printf("%s Hairdresser [%d] finished\n", time_msg, id);
		sem_buf.sem_num = CHAIR_SEM_ID;
		sem_buf.sem_op = 1;
		semop(semDesc, &sem_buf, 1);
	}
}

int main(int argc, char** argv) {
	if (argc < 5) {
		fprintf(stderr, "Error | correct input pattern [M N P F]\n");
		exit(EXIT_FAILURE);
	}

	M = atoi(argv[1]);
	N = atoi(argv[2]);
	P = atoi(argv[3]);
	F = atoi(argv[4]);
	if (M < 1 || N < 1 || P < 1 || F < 1) {
		fprintf(stderr, "Error | incorrect argument\n");
		exit(EXIT_FAILURE);
	}
	if (M < N) {
		fprintf(stderr, "M must be greater than N\n");
		exit(EXIT_FAILURE);
	}

	setbuf(stdout, NULL);
	srand(time(NULL));

	signal(SIGINT, signal_handler);

	service_times = malloc(F * sizeof(int));

	// initializing service times //
	printf(">>> SERVICES: <<<\n");
	for (int i = 0; i < F; i++) {
		service_times[i] = 1000 + random() % 4000;
		printf("id: %d, time: %fs\n", i+1, service_times[i]/1000.0);
	}
	printf("\n");
	////////////////////////////////

	key = ftok("salon.c", 1 + rand()%255);
	if ((semDesc = semget(key, 3, IPC_CREAT | 0666)) == -1) {
		perror("semget");
		exit(EXIT_FAILURE);
	}
	if ((shmDesc = shmget(key, (P+1)*sizeof(int), IPC_CREAT | 0666)) == -1) {
		perror("shmget");
		exit(EXIT_FAILURE);
	}
	semctl(semDesc, CHAIR_SEM_ID, SETVAL, N);
	semctl(semDesc, WAITING_SEM_ID, SETVAL, 0);
	semctl(semDesc, TIMES_SEM_ID, SETVAL, 1);
	sem_buf.sem_flg = 0;

	int* times = (int*) shmat(shmDesc, NULL, 0); // ns
	memset(times, -1, (P+1)*sizeof(int));
	times[0] = 0;
	shmdt(times);

	for (int i = 0; i < M; i++) {
		if (fork() == 0) {
			signal(SIGINT, hairdresser_sig_handler);
			hairdresser_loop(i);
			exit(EXIT_SUCCESS);
		}
	}

	client_loop();

	while(wait(NULL) > 0);
	free(service_times);
	semctl(semDesc, 0, IPC_RMID);
	shmdt(times);
	shmctl(shmDesc, IPC_RMID, NULL);

	exit(EXIT_SUCCESS);
}
