#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wc.h"

int main() {
	char line[100];
	char *command, *arg;
	int size, index;
	clock_t start_time, end_time;
	int init_flag = 0;

	FilesInfo arr;

	while(1) {
		printf("> ");
		if (fgets(line, sizeof(line), stdin) == NULL) {
			break; // zrzut pamięci?
		}

		command = strtok(line, " \n");
		arg = strtok(NULL, " \n");

		if (strcmp(command, "exit") == 0) {
			break;
		}
		else if (strcmp(command, "init") == 0 && !init_flag) {
			arr = init(atoi(arg));
			init_flag = 1;
			printf("%s array was initialized!\n", arg);
		}
		else if (strcmp(command, "count") == 0 && init_flag) {
			count(&arr, arg);
			printf("%s was counted!\n", arg);
		}
		else if (strcmp(command, "show") == 0 && init_flag) {
			printf("%s: %s\n", arg, show(arr, atoi(arg)));
		}
		else if (strcmp(command, "delete") == 0 && init_flag) { //delete index
			delete_idx(&arr, atoi(arg));
			printf("index %s has been deleted!\n", arg);
		}
		else if (strcmp(command, "destroy") == 0 && init_flag) {
			destroy(&arr);
			init_flag = 0;
			printf("array has been destroyed!\n");
		}
		else if (!init_flag) {
			printf("you need initialize arr first!\n");
		}
		else {
			printf("wrong command!\n");
		}
	}
}