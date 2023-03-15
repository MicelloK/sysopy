#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <dirent.h>

#define BUFFER_SIZE 256

void search_directory(char* dir_path, char* search_string);

int main(int argc, char* argv[]) {
	if (argc != 3) {
		fprintf(stderr, "Wrong number of arguments!\n");
		exit(EXIT_FAILURE);
	}

	search_directory(argv[1], argv[2]);

	while(wait(NULL) > 0);

	return EXIT_SUCCESS;
}

void search_directory(char* dir_path, char* search_string) {
	DIR* dir;
	struct dirent *entry;
	struct stat stat_buf;
	char file_path[PATH_MAX];
	pid_t pid;

	if ((dir = opendir(dir_path)) == NULL) {
		perror("opendir");
		exit(EXIT_FAILURE);
	}

	while ((entry = readdir(dir)) != NULL) {
		if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
			continue;
		}

		snprintf(file_path, PATH_MAX, "%s/%s", dir_path, entry->d_name);

		if (lstat(file_path, &stat_buf) == -1) {
			perror("lstat");
			exit(EXIT_FAILURE);
		}

		// katalog
		if (S_ISDIR(stat_buf.st_mode)) {
			pid = fork();
			if (pid == -1) {
				perror("fork");
				exit(EXIT_FAILURE);
			}
			else if (pid == 0) {
				search_directory(file_path, search_string);
				exit(EXIT_SUCCESS);
			}
		}
		// plik
		else {
			FILE* file;
			char* line = NULL;
			size_t len = 0;

			if ((file = fopen(file_path, "r")) == NULL) {
				perror("open");
				exit(EXIT_FAILURE);
			}

			getline(&line, &len, file);
			if (strncmp(line, search_string, strlen(search_string)) == 0) {
				printf("PID: %d, %s\n", getpid(), realpath(file_path, NULL));
			}

			free(line);
			fclose(file);
		}
	}

	if (closedir(dir) == -1) {
		perror("closedir");
		exit(EXIT_FAILURE);
	}
}