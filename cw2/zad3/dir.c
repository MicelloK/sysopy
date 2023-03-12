#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

int main() {
	DIR* dir;
	struct dirent *entry;
	struct stat buf;
	long long total_size = 0;

	dir = opendir(".");
	if (dir == NULL) {
		printf("Error opening current dir\n");
		return -1;
	}

	while ((entry = readdir(dir)) != NULL) {
		if (entry->d_type == DT_REG) {
			if (stat(entry->d_name, &buf) == -1) {
				printf("Stat Error!\n");
				return -1;
			}

			if (!S_ISDIR(buf.st_mode)) {
				printf("%s: %lld\n", entry->d_name, (long long)buf.st_size);
				total_size += buf.st_size;
			}
		}
	}

	printf("TOTAL SIZE: %lld\n", total_size);
	closedir(dir);
	return 0;
}