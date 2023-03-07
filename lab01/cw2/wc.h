#ifndef WC_H_INCLUDED
#define WC_H_INCLUDED

typedef struct FilesInfo {
	char** data;
	int current_size;
	int max_size;
} FilesInfo;

FilesInfo init(int size);

void count(FilesInfo* arr, char* file);

char* show(FilesInfo arr, int index);

void delete_idx(FilesInfo* arr, int index);

void destroy(FilesInfo* arr);

#endif
