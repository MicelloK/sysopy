#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

int sig_recieved;

void handler(int signum) {
    sig_recieved = 1;
}

int main(int argc, char** argv) {
    if (argc < 3) {
        fprintf(stderr, "Wrong arguments number!\n");
        exit(EXIT_FAILURE);
    }

    int state;
    int catcher_pid = atoi(argv[1]);

    for (int i = 2; i < argc; i++) {
        sig_recieved = 0;

        state = atoi(argv[i]);

        struct sigaction action;
        sigemptyset(&action.sa_mask);
        action.sa_handler = handler;
        sigaction(SIGUSR1, &action, NULL);

        sigval_t sig_val = {state};
        sigqueue(catcher_pid, SIGUSR1, sig_val);
        printf("Signal sent..  | State: %d\n", state);

        while(!sig_recieved);
        printf("Signal recieved..\n");
    }
}