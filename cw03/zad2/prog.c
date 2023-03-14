#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

char* file_name(char* path);

int main(int argc, char* argv[]) {
	if (argc != 2) {
		fprintf(stderr, "Wrong arguments amount!\n");
		exit(EXIT_FAILURE);
	}

	setbuf(stdout, NULL);

	printf("%s ", file_name(argv[0]));
	fflush(stdout);

	if (execl("/bin/ls", "ls", argv[1], NULL) == -1) {
		perror("execl");
		exit(EXIT_FAILURE);
	}

	return EXIT_SUCCESS;
}

char* file_name(char* path) {
	char* last_slash = strrchr(path, '/');
	if (last_slash != NULL) {
		return (last_slash + 1);
	}
	return NULL;
}