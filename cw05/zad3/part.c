#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 1024
char buffer[BUFFER_SIZE] = "";

double fun(double x) {
	return 4 / (x*x + 1);
}

double int_part(double begin, double end, double dx) {
	double current_pos = 0.0;
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
	double begin = strtod(argv[1], NULL);
	double end = strtod(argv[2], NULL);
	double dx = strtod(argv[3], NULL);

	double res = int_part(begin, end, dx);
	ssize_t size = snprintf(buffer, BUFFER_SIZE, "%f\n", res);

	int fifo = open("/tmp/int_q", O_WRONLY);
	write(fifo, buffer, size);
	close(fifo);
	exit(EXIT_SUCCESS);
}