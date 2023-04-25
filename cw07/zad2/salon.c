#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <time.h>
#include <signal.h>

char* ch_sem_name = "/chair";
char* w_sem_name = "/waiting";
char* m_sem_name = "/mem";
char* mem_name = "/times";

int M, N, P, F;
int *service_times; // ns
int shmDesc;
int run = 1;
char time_msg[16];
int* times;

void set_time() {
	struct tm time_struct;
	time_t my_time = time(NULL);
	time_struct = *localtime(&my_time);
	sprintf(time_msg, "[%02d:%02d:%02d]", time_struct.tm_hour, time_struct.tm_min, time_struct.tm_sec);
}

void signal_handler(int signum) {
	sem_unlink("/chair");
	sem_unlink("/waiting");
	sem_unlink("/mem");
	shm_unlink("/times");
	exit(EXIT_SUCCESS);
}

void hairdresser_loop(int id) {
	sem_t *wait_sem = sem_open(w_sem_name, 0);
	sem_t *chair_sem = sem_open(ch_sem_name, 0);
	sem_t *mem_sem = sem_open(m_sem_name, 0);

	shmDesc = shm_open(mem_name, O_RDWR, 0666);

	set_time();
	printf("%s Hairdresser [%d] started working...\n", time_msg, id);
	while(run) {
		sem_wait(chair_sem);
		sem_wait(wait_sem);

		set_time();
		printf("%s Hairdresser [%d] has reserved chair, taking client..\n", time_msg, id);

		sem_wait(mem_sem);
		times = mmap(NULL, (P+1)*sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shmDesc, 0);

		int time = times[times[0]+1];
		times[times[0]+1] = -1;
		times[0] = (times[0] + 1) % P;

		munmap(times,sizeof(int)*(P+1));
		sem_post(mem_sem);

		usleep(time);
		set_time();
		printf("%s Hairdresser [%d] finished\n", time_msg, id);
		sem_post(chair_sem);
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

	sem_t *chair_sem = sem_open(ch_sem_name, O_CREAT, 0666, 0);
	sem_t *mem_sem = sem_open(m_sem_name, O_CREAT, 0666, 0);
	sem_t *wait_sem = sem_open(w_sem_name, O_CREAT, 0666, 0);
	for (int i = 0; i < N; i++) {
		sem_post(chair_sem);
	}
	sem_post(mem_sem);

	sem_close(chair_sem);

	int shmDesc = shm_open(mem_name, O_CREAT | O_RDWR, 0666);
	ftruncate(shmDesc, (P+1)*sizeof(int));
	times = (int*) mmap(NULL, sizeof(int)*(P+1), PROT_READ | PROT_WRITE, MAP_SHARED, shmDesc, 0);
	memset(times, -1, (P+1)*sizeof(int));
	times[0] = 0;
	munmap(times, (P+1)*sizeof(int));

	for (int i = 0; i < M; i++) {
		if (fork() == 0) {
			signal(SIGINT, signal_handler);
			hairdresser_loop(i);
			exit(EXIT_SUCCESS);
		}
	}

	// client loop //
	int last_client = 0;
	while(run) {
		sleep(rand() % 3);
		int service_number = rand()%F;
		int time = service_times[service_number];
		set_time();
		printf("%s New client has arrived. Service number: %d, time: %fs\n", time_msg, service_number+1, time/1000.0);

		sem_wait(mem_sem);
		times = mmap(NULL, (P+1)*sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shmDesc, 0);
		if (times[last_client+1] == -1) {
			times[last_client+1] = time;
			last_client = (last_client + 1) % P;
			sem_post(wait_sem);

			set_time();
			printf("%s Client take place in the queue\n", time_msg);
		}
		else {
			set_time();
			printf("%s No empty place in queue, leaving..\n", time_msg);;
		}

		munmap(times, sizeof(int)*(P+1));
		sem_post(mem_sem);
	}

	while(wait(NULL) > 0);
	free(service_times);
	exit(EXIT_SUCCESS);
}
