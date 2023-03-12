#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wc.h"

FilesInfo init(int size) {
    char** data = calloc(size, sizeof(char*));
    struct FilesInfo new_array = {data, 0, size};
    printf("[%d] has been initialized!\n", size);
    return new_array;
}

void count(FilesInfo* arr, char* file) {
    if (arr->current_size < arr->max_size) { 
        // wc -> tmp
        char* command = malloc(strlen("wc  > /tmp/tmp_file") + strlen(file) + 1);
        sprintf(command, "wc %s > /tmp/tmp_file", file);
        system(command);

        // file read
        FILE* tmp_file = fopen("/tmp/tmp_file", "r");
        if (tmp_file == NULL) {
            printf("can not open tmp file!\n");
        }

        // data cpy
        fseek(tmp_file, 0L, SEEK_END);
        long int tmp_file_size = ftell(tmp_file);
        fseek(tmp_file, 0L, SEEK_SET);

        arr->data[arr->current_size] = malloc(tmp_file_size);
        fgets(arr->data[arr->current_size], tmp_file_size, tmp_file);

        arr->current_size++;

        // del tmp file
        remove("/tmp/tmp_file");  //nie dziala chyba

        printf("wc %s\n", file);
    }
}

char* show(FilesInfo arr, int index) {
    if (index >= 0 && index < arr.current_size) {
        return arr.data[index];
    }
    return NULL;
}

void delete_idx(FilesInfo* arr, int index) {
    if (index >= 0 && index < arr->current_size) {
        free(arr->data[index]);
        while(index < arr->current_size - 1) {
            arr->data[index] = arr->data[index + 1];
            index++;
        } 
        arr->data[index] = NULL;
        arr->current_size--;
    }

    printf("index %d has been deleted!\n", index);
}

void destroy(FilesInfo* arr) {
    for (int i = 0; i < arr->current_size; i++) {
        free(arr->data[i]);
    }
    free(arr->data);

    printf("Array has been destroyed!\n");
}
