#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
	if (argc != 2) {
		fprintf(stderr, "Wrong arguments amount!\n");
		exit(EXIT_FAILURE);
	}

	int procs_num = atoi(argv[1]);

	for (int i = 0; i < procs_num; i++) {
		pid_t pid = fork();

		if (pid == -1) {
			perror("fork");
			exit(EXIT_FAILURE);
		}
		else if (pid == 0) {
			printf("Parrent process ID: %d, Child process ID: %d\n", getppid(), getpid());
			exit(EXIT_SUCCESS);
		}
	}

	for (int i = 0; i < procs_num; i++) {
		if (wait(NULL) == -1) {
			perror("wait");
			exit(EXIT_FAILURE);
		}	
	}

	printf("Number of forks: %d\n", procs_num);
	return EXIT_SUCCESS;
}