#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/times.h>

#ifndef DLL
#include "wc.h"
#endif

#ifdef DLL
#include <dlfcn.h>

typedef struct FilesInfo {
	char** data;
	int current_size;
	int max_size;
} FilesInfo;
#endif


int main() {
	#ifdef DLL
	void *handle = dlopen("../zad1/libwc.so", RTLD_LAZY);
	if (handle == NULL) {
		printf("Cannot find library..\n");
		return -1;
	}

	struct FilesInfo (*init)(int) = dlsym(handle, "init");
	void (*count)(struct FilesInfo*, char*) = dlsym(handle, "count");
	char* (*show)(struct FilesInfo, int) = dlsym(handle, "show");
	void (*delete_idx)(struct FilesInfo*, int) = dlsym(handle, "delete_idx");
	void (*destroy)(struct FilesInfo*) = dlsym(handle, "destroy");
	#endif

	char *command, *arg;
	int init_flag = 0;

	struct timespec start_time, end_time, res;
	clockid_t clk_id = CLOCK_REALTIME;
	double real_time, user_time, sys_time;

	FilesInfo arr;

	clock_getres(clk_id, &res);
	printf("Clock resolution: %ld ns\n", res.tv_nsec);

	while(1) {
		char line[100];
		printf("> ");
		if (fgets(line, sizeof(line), stdin) == NULL) { // zrzut pamięci?
			continue;
		}

		command = strtok(line, " \n");
		arg = strtok(NULL, " \n");

		if (strcmp(command, "exit") == 0) {
			printf("program has ended.");
			break;
		}
		else if (strcmp(command, "init") == 0) {
			if (init_flag) {
				printf("The array has already been initialized.\n");
				continue;
			}

			if (arg == NULL) {
				printf("Not enougth arguments.\n");
				continue;
			}

			clock_gettime(clk_id, &start_time);
			arr = init(atoi(arg));
			clock_gettime(clk_id, &end_time);

			init_flag = 1;
		}
		else if (strcmp(command, "count") == 0 && init_flag) {
			if (arg == NULL) {
				printf("Not enougth arguments.\n");
				continue;
			}

			clock_gettime(clk_id, &start_time);
			count(&arr, arg);
			clock_gettime(clk_id, &end_time);
		}
		else if (strcmp(command, "show") == 0 && init_flag) {
			if (arg == NULL) {
				printf("Not enougth arguments.\n");
				continue;
			}

			clock_gettime(clk_id, &start_time);
			printf("%s: %s\n", arg, show(arr, atoi(arg)));
			clock_gettime(clk_id, &end_time);
		}
		else if (strcmp(command, "delete") == 0 && init_flag) {
			if (arg == NULL) {
				printf("Not enougth arguments.\n");
				continue;
			}

			clock_gettime(clk_id, &start_time);
			delete_idx(&arr, atoi(arg));
			clock_gettime(clk_id, &end_time);
		}
		else if (strcmp(command, "destroy") == 0 && init_flag) {
			clock_gettime(clk_id, &start_time);
			destroy(&arr);
			clock_gettime(clk_id, &end_time);

			init_flag = 0;
		}
		else if (!init_flag) {
			printf("you need to initialize array first!\n");
			continue;
		}
		else {
			printf("unknown command!\n");
			continue;
		}

		real_time = (end_time.tv_sec - start_time.tv_sec) + (end_time.tv_nsec - start_time.tv_nsec) / 1e9;
		printf("REAL TIME: %f s\n", real_time);

		struct tms buffer;
		times(&buffer);
		user_time = (double) buffer.tms_utime / CLOCKS_PER_SEC;
		sys_time = (double) buffer.tms_stime / CLOCKS_PER_SEC;
		printf("USER TIME: %f s\n", user_time);
		printf("SYSTEM TIME: %f s\n", sys_time); 
	}
	
	#ifdef DLL
	dlclose(handle);
	#endif
}
