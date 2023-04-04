#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#define BUFFER_SIZE 64

char buffer[BUFFER_SIZE];
double current_pos = 0.0;

double fun(float x) {
	return 4/(x*x+1);
}

double int_part(double begin, double end, double dx) {
	while (current_pos < begin) {
		current_pos += dx;
	}

	double sum = 0.0;
	while (current_pos < end) {
		sum += fun(current_pos) * dx;
		current_pos += dx;
	}
	return sum;
}

int main(int argc, char** argv) {
	if (argc != 3) {
		fprintf(stderr, "Wrong number of arguments!\n");
		exit(EXIT_FAILURE);
	}

	clock_t start, end;
	double cpu_time_used;
	start = clock();

	double dx = strtod(argv[1], NULL);
	int n = atoi(argv[2]);
	if (n*dx > 1) {
		n = 1 / dx;
	}

	double n_space = 1.0 / n;

	int fds[n];

	for (int i = 0; i < n; i++) {
		int fd[2];
		pipe(fd);
		if (fork() == 0) {
			close(fd[0]);
			double result = int_part(i*n_space, (i+1)*n_space, dx);
			ssize_t size = snprintf(buffer, BUFFER_SIZE, "%f", result);
			write(fd[1], buffer, size);
			exit(EXIT_SUCCESS);
		}
		else {
			close(fd[1]);
			fds[i] = fd[0];
		}
	}

	while(wait(NULL) > 0);

	double sum = 0.0;

	for (int i = 0; i < n; i++) {
		ssize_t size = read(fds[i], buffer, BUFFER_SIZE);
		buffer[size] = '\0';
		sum += strtod(buffer, NULL);
	}

	end = clock();
	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;


	printf(">> RESULT <<\n");
	printf("sum = %lf\nn = %d\ndx = %lf\n", sum, n, dx);
	printf("TIME USED: %fs\n", cpu_time_used);
	printf("-------------\n");
	exit(EXIT_SUCCESS);
}