#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 1024

int lib_implementation(char a, char b, char* input_filename, char* output_filename);
int sys_implementation(char a, char b, char* input_filename, char* output_filename);

int main(int argc, char* argv[]) {
	if (argc != 5) {
		printf("Wrong arguments amount!\n");
		return -1;
	}

	char a = argv[1][0];
	char b = argv[2][0];
	char* input_filename = argv[3];
	char* output_filename = argv[4];

	char* report_file = "pomiar_zad_1.txt";
	FILE* report = fopen(report_file, "wa");

	clock_t start, end;
	int count = 1;

	// lib implementation
	start = clock();
	count = lib_implementation(a, b, input_filename, output_filename);
	end = clock();

	if (count == -1) {
		fprintf(report, "LIB IMPLEMENTATION:\nError opening input file: %s!\n", input_filename);
	}
	else {	
		fprintf(report, "LIB IMPLEMENTATION:\nCharacter replaced: %d\nExecution time: %f seconds\n", count, (double)(end - start) / CLOCKS_PER_SEC);
	}

	// sys implementation
	start = clock();
	// count = sys_implementation(a, b, input_filename, output_filename);
	end = clock();

	if (count == -1) {
		fprintf(report, "SYS IMPLEMENTATION:\nError opening input file: %s!\n", input_filename);
	}
	else {	
		fprintf(report, "SYS IMPLEMENTATION:\nCharacter replaced: %d\nExecution time: %f seconds\n", count, (double)(end - start) / CLOCKS_PER_SEC);
	}

	fclose(report);
	return 0;
}

int lib_implementation(char a, char b, char* input_filename, char* output_filename) {
	FILE* input_file = fopen(input_filename, "rb");
	if (!input_file) {
		printf("Error opening input file: %s!\n", input_filename);
		return -1;
	}

	FILE* output_file = fopen(output_filename, "wb");
	if (!output_file) {
		printf("Error opening output file: %s!\n", output_filename);
		return -1;
	}

	int count = 0;
	while(!feof(input_file)) {
		char buffer[BUFFER_SIZE];
		size_t read_count = fread(buffer, sizeof(char), BUFFER_SIZE, input_file);
		for(size_t i = 0; i < read_count; i++) {
			if (buffer[i] == a) {
				buffer[i] = b;
				count++;
			}
		}
		fwrite(buffer, sizeof(char), read_count, output_file);
	}

	fclose(input_file);
	fclose(output_file);
	return count;
}

int sys_implementation(char a, char b, char* input_filename, char* output_filename) {
	int input_fd = open(input_filename, O_RDONLY);
	if (input_fd == -1) {
		printf("Error opening input file: %s!\n", input_filename);
		return -1;
	}

	int output_fd = open(output_filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (output_fd == -1) {
		printf("Error opening output file: %s!\n", output_filename);
		return -1;
	}

	char buffer[BUFFER_SIZE];
	ssize_t read_count;

	int count = 0;
	while((read_count = read(input_fd, buffer, BUFFER_SIZE)) > 0) {
		for(ssize_t i = 0; i < read_count; i++) {
			if (buffer[i] == a) {
				buffer[i] = b;
				count++;
			}
		}
		if (write(output_fd, buffer, read_count) == read_count) {
			printf("Error writing to output file: %s!\n", output_filename);
            return -1;
		}
	}
	if (read_count == -1) {
        printf("Error reading input file: %s!\n", input_filename);
        return -1;
    }

    if (close(input_fd) == -1) {
        printf("Error closing input file: %s!\n", input_filename);
        return -1;
    }
    if (close(output_fd) == -1) {
        printf("Error closing output file: %s!\n", output_filename);
        return -1;
    }
	return count;
}
