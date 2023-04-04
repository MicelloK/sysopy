#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1024
char buffer[BUFFER_SIZE] = "";
char begin[BUFFER_SIZE] = "";
char end[BUFFER_SIZE] = "";

int main(int argc, char** argv) {
	if (argc != 3) {
		fprintf(stderr, "Wrong number of arguments!\n");
		exit(EXIT_FAILURE);
	}

	clock_t cl_start, cl_end;
	double cpu_time_used;
	cl_start = clock();

	double dx = strtod(argv[1], NULL);
	int n = atoi(argv[2]);
	if (n*dx > 1) {
		n = 1 / dx;
	}

	double n_space = 1.0 / n;

	mkfifo("/tmp/int_q", 0666);

	for (int i = 0; i < n; i++) {
		if (!fork()) {
			snprintf(begin, BUFFER_SIZE, "%lf", i*n_space);
			snprintf(end, BUFFER_SIZE, "%lf", (i+1)*n_space);
			execl("./part", "part", begin, end, argv[1], NULL);
		}
	}

	double sum = 0.0;

	int fifo = open("/tmp/int_q", O_RDONLY);
	int read_count = 0;

	while (read_count < n) {
		wait(NULL);
		ssize_t size = read(fifo, buffer, BUFFER_SIZE);
		buffer[size] = '\0';

		char* token = strtok(buffer, "\n");
		while (token != NULL) {
			sum += strtod(token, NULL);
			read_count++;
			token = strtok(NULL, "\n");
		}
	}

	close(fifo);

	cl_end = clock();
	cpu_time_used = ((double) (cl_end - cl_start) / CLOCKS_PER_SEC);

	printf(">> RESULT <<\n");
	printf("sum = %lf\nn = %d\ndx = %lf\n", sum, n, dx);
	printf("TIME USED: %fs\n", cpu_time_used);
	printf("-------------\n");
	exit(EXIT_SUCCESS);
}