#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>

int is_parent = 1;

void mask_signal() {
    sigset_t mask_set;
    sigemptyset(&mask_set);
    sigaddset(&mask_set, SIGUSR1);
    if (sigprocmask(SIG_BLOCK, &mask_set, NULL) == -1) {
        perror("sigprocmask");
        exit(EXIT_FAILURE);
    }
}

void check_pending() {
    sigset_t pending_set;
    if (sigpending(&pending_set) == -1) {
        perror("sigpending");
        exit(EXIT_FAILURE);
    }
    if (sigismember(&pending_set, SIGUSR1)) {
        printf("(%d) Signal is pending\n", getpid());
    }
    else {
        printf("(%d) Signal is not pending\n", getpid());
    }
}

void raise_signal() {
    printf("(%d) Raising signal\n", getpid());
    raise(SIGUSR1);
}

void process_signal(char* parse_option) {
    if (strcmp(parse_option, "ignore") == 0) {
        signal(SIGUSR1, SIG_IGN);
        raise_signal();
    }
    else if (strcmp(parse_option, "mask") == 0) {
        if (is_parent) {
        	mask_signal();
        	raise_signal();
        }
    }
    else if (strcmp(parse_option, "pending") == 0) {
        if (is_parent) {
            mask_signal();
            raise_signal();
        }
        check_pending();
    }
    else {
        fprintf(stderr, "Unknown argument: %s\n", parse_option);
        exit(EXIT_FAILURE);
    }
    fflush(NULL);
}

int main(int argc, char** argv) {
    if (argc != 2 && argc != 3) {
        fprintf(stderr, "Wrong arguments number!\n");
        exit(EXIT_FAILURE);
    }
    if (argc == 3 && strcmp(argv[2], "execl") != 0) {
        fprintf(stderr, "Wrong argv[2] set!\n");
        exit(EXIT_FAILURE);
    }
    if (argc == 3) {
        is_parent = 0;
    }

    process_signal(argv[1]);

    if (is_parent) {
        execl(argv[0], argv[0], argv[1], "execl", NULL);
    }

    exit(EXIT_SUCCESS);
}