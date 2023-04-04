#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 1024
char buffer[BUFFER_SIZE];

int main(int argc, char** argv) {
	if (argc < 2) {
		fprintf(stderr, "Wrong number of arguments!\n");
		exit(EXIT_FAILURE);
	}

	if ((strcmp(argv[1], "write") == 0 || strcmp(argv[1], "w") == 0) && argc != 3) {
		fprintf(stderr, "Wrong number of arguments!\n");
		exit(EXIT_FAILURE);
	}
	else if ((strcmp(argv[1], "write") == 0 || strcmp(argv[1], "w") == 0) && argc == 3) {
		FILE* input = popen("cowsay", "w");
		fwrite(argv[2], sizeof(char), strlen(argv[2]), input);
		pclose(input);
	}
	else if (strcmp(argv[1], "read") == 0 || strcmp(argv[1], "r") == 0) {
		FILE* output = popen("fortune", "r");
		ssize_t size = fread(buffer, sizeof(char), BUFFER_SIZE, output);
		buffer[size] = '\0';
		printf("%s\n", buffer);
		pclose(output);
	}
	else {
		fprintf(stderr, "Unknown argument!\n![w/write/r/read]\n");
		exit(EXIT_FAILURE);
	}
}