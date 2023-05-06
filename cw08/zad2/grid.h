#pragma once
#include <stdbool.h>

char *create_grid();
void destroy_grid(char *grid);
void draw_grid(char *grid);
void init_grid(char *grid);
bool is_alive(int row, int col, char *grid);
void update_grid(char *src, char *dst, int no_threads);

typedef struct {
	char* src;
	char* dst;
	int start_idx;
	int block_size;
} UpdateArgs;
