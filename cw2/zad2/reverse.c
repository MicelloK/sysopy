#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BUFFER_SIZE 1024

void block_reverse(FILE* input_file, FILE* output_file);
void character_reverse(FILE* input_file, FILE* output_file);

int main(int argc, char* argv[]) {
	if (argc != 3) {
		printf("Wrong arguments amount!");
		return -1;
	}

	char* input_filename = argv[1];
	char* output_filename = argv[2];

	char* report_filename = "pomiar_zad_2.txt";
	FILE* report_file = fopen(report_filename, "wa");

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

	clock_t start, end;

	start = clock();
	block_reverse(input_file, output_file);
	end = clock();
	fprintf(report_file, "BLOCK IMPLEMENTATION:\nExecution time: %f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);

	start = clock();
	character_reverse(input_file, output_file);
	end = clock();
	fprintf(report_file, "CHARACTER IMPLEMENTATION:\nExecution time: %f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);

	fclose(input_file);
	fclose(output_file);
	fclose(report_file);
	return 0;
}

void block_reverse(FILE* input_file, FILE* output_file) {
	fseek(input_file, 0, SEEK_END);
	long file_size = ftell(input_file);
	fseek(output_file, 0, SEEK_SET);

	char buffer[BUFFER_SIZE];
	for(int i = file_size - 1; i >= 0; i -= BUFFER_SIZE) {
		fseek(input_file, i, SEEK_SET);
		size_t read_count = fread(buffer, sizeof(char), BUFFER_SIZE, input_file);
		char rev_buffer[BUFFER_SIZE];
		for(size_t j = 0; j < read_count; j++) {
			rev_buffer[j] = buffer[read_count - 1 - i];
		}
		fwrite(rev_buffer, sizeof(char), read_count, output_file);
	}
}

void character_reverse(FILE* input_file, FILE* output_file) {
	fseek(input_file, 0, SEEK_END);
	long file_size = ftell(input_file);
	fseek(input_file, 0, SEEK_SET);

	char* buffer = malloc(sizeof(char));
	for(int i = file_size - 1; i >= 0; i--) {
		fseek(input_file, i, SEEK_SET);
		fread(buffer, sizeof(char), 1, input_file);
		fwrite(buffer, sizeof(char), 1, output_file);
	}
}