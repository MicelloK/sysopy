#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>

int requests_num = 0;
int catcher_state = 0;
int done;

void handler (int signum, siginfo_t* info, void* context) {
	int pid = info->si_pid;
	catcher_state = info->si_status;

	if (catcher_state < 1 || catcher_state > 5) {
		fprintf(stderr, "CATCHER | There is no such mode of operation!\n");
	}
	else {
		requests_num++;
		done = 0;
	}

	kill(pid, SIGUSR1);
}

void print_numbers() {
	for (int i = 0; i < 100; i++) {
		printf("CATCHER | %d\n", i+1);
	}
	done = 1;
}

void print_time() {
	static time_t raw_time;
	static struct tm* time_info;
	time(&raw_time);
	time_info = localtime(&raw_time);
	printf("CATCHER | Current time: %s", asctime(time_info));
	done = 1;
}

void print_requests_num() {
	printf("CATCHER | Requests count: %d\n", requests_num);
	done = 1;
}

void end_prog() {
	printf("CATCHER | Exit\n");
	exit(EXIT_SUCCESS);
}

int main(int argc, char** argv) {
	struct sigaction action;
	sigemptyset(&action.sa_mask);
	action.sa_sigaction = handler;
	action.sa_flags = SA_SIGINFO;
	sigaction(SIGUSR1, &action, NULL);

	printf("CATCHER | PID: %d\n", getpid());

	while(1) {
		if (done) pause();

		if (catcher_state == 1) {
			print_numbers();
		}
		else if (catcher_state == 2) {
			print_time();
		}
		else if (catcher_state == 3) {
			print_requests_num();
		}
		else if (catcher_state == 4) {
			print_time();
			sleep(1);
			done = 0;
		}
		else if (catcher_state == 5) {
			end_prog();
		}
		fflush(NULL);
	}

	exit(EXIT_SUCCESS);
}