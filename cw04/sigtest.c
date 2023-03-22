#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>

void signal_handler(int signum);
void check_pending(char* signal_name);

int main(int argc, char* argv[]) {
	if (argc != 2) {
		fprintf(stderr, "Wrong arguments amount!\n");
		exit(EXIT_FAILURE);
	}

	if (strcmp(argv[1], "ignore") == 0) {
		signal(SIGUSR1, SIG_IGN);
	}
	else if (strcmp(argv[1], "handler") == 0) {
		signal(SIGUSR1, signal_handler);
	}
	else if (strcmp(argv[1], "mask") == 0) {
		sigset_t mask_set;
		sigemptyset(&mask_set);
		sigaddset(&mask_set, SIGUSR1);
		if (sigprocmask(SIG_BLOCK, &mask_set, NULL) == -1) {
			perror("sigprocmask");
			exit(EXIT_FAILURE);
		}
	}
	else if (strcmp(argv[1], "pending") == 0) {
		check_pending("Parent");
		raise(SIGUSR1);
		printf("Sent SIGUSR1 to self\n");
		check_pending("Parent");
	}
	else {
		fprintf(stderr, "Unknown argument: %s\n", argv[1]);
		exit(EXIT_FAILURE);
	}

	pid_t child_pid = fork();
	if (child_pid == -1) {
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if (child_pid == 0) {
		if (strcmp(argv[1], "pending") == 0) {
			check_pending("Child");
		}
		raise(SIGUSR1);
		printf("Sent SIGUSR1 to self\n");
		if (strcmp(argv[1], "ignore") == 0) {
			printf("Child: Ignoring SIGUSR1\n");
		}
		else if (strcmp(argv[1], "handler") == 0) {
			printf("Child: Handling SIGUSR1\n");
		}
		else if (strcmp(argv[1], "mask") == 0) {
			printf("Child: Masking SIGUSR1\n");
		}
		else if (strcmp(argv[1], "pending") == 0) {
			check_pending("Child");
		}
	}

	wait(NULL);
	exit(EXIT_SUCCESS);
}

void signal_handler(int signum) {
	printf("Signal number: %d\n", signum);
}

void check_pending(char* signal_name) {
	sigset_t pending_set;
	if (sigpending(&pending_set) == -1) {
		perror("sigpending");
		exit(EXIT_FAILURE);
	}
	if (sigismember(&pending_set, SIGUSR1)) {
		printf("%s: SIGUSR1 is pending\n", signal_name);
	}
	else {
		printf("%s: SIGUSR1 is not pending\n", signal_name);
	}
}