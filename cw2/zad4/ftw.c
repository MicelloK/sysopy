#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <ftw.h>

int parseFile(const char *path, const struct stat *statbuf, int typefile __attribute__((unused)));

long long total_size = 0;

int main(int argc, char* argv[]) {
	if (argc != 2) {
		printf("Wrong arguments amount!\n");
		return -1;
	}

	char* path = argv[1];

	if (ftw(path, parseFile, 16) == -1) {
		printf("Error ftw\n");
		return -1;
	}

	printf("TOTAL SIZE: %lld\n", total_size);
}

int parseFile(const char *path, const struct stat *statbuf, int typefile __attribute__((unused))) {
	if (!S_ISDIR(statbuf->st_mode)) {
		printf("%s: %ld\n", path, statbuf->st_size);
		total_size += (long long)statbuf->st_size;
	}
	return 0;
}
