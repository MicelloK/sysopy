#include "grid.h"
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>

const int grid_width = 30;
const int grid_height = 30;
const int size = grid_width * grid_height;

char *create_grid()
{
    return malloc(sizeof(char) * grid_width * grid_height);
}

void destroy_grid(char *grid)
{
    free(grid);
}

void draw_grid(char *grid)
{
    for (int i = 0; i < grid_height; ++i)
    {
        // Two characters for more uniform spaces (vertical vs horizontal)
        for (int j = 0; j < grid_width; ++j)
        {
            if (grid[i * grid_width + j])
            {
                mvprintw(i, j * 2, "■");
                mvprintw(i, j * 2 + 1, " ");
            }
            else
            {
                mvprintw(i, j * 2, " ");
                mvprintw(i, j * 2 + 1, " ");
            }
        }
    }

    refresh();
}

void init_grid(char *grid)
{
    for (int i = 0; i < grid_width * grid_height; ++i)
        grid[i] = rand() % 2 == 0;
}

bool is_alive(int row, int col, char *grid)
{

    int count = 0;
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            if (i == 0 && j == 0)
            {
                continue;
            }
            int r = row + i;
            int c = col + j;
            if (r < 0 || r >= grid_height || c < 0 || c >= grid_width)
            {
                continue;
            }
            if (grid[grid_width * r + c])
            {
                count++;
            }
        }
    }

    if (grid[row * grid_width + col])
    {
        if (count == 2 || count == 3)
            return true;
        else
            return false;
    }
    else
    {
        if (count == 3)
            return true;
        else
            return false;
    }
}

void swap(UpdateArgs* args) {
    char* tmp = args->src;
    args->src = args->dst;
    args->dst = tmp;
}

void empty_handler(int sig) {} // ignore

void* single_update(void* upd_args) {
    UpdateArgs* args = (UpdateArgs*) upd_args;

    while(1) {
        for(int i = args->start_idx; i < args->start_idx + args->block_size; i++) {
            args->dst[i] = is_alive(i / grid_width, i % grid_width, args->src);
        }
        pause();
        swap(args);
    }
}

void update_grid(char *src, char *dst, int no_threads)
{
    static pthread_t* threads = NULL;

    if (!threads) {
        struct sigaction action;
        sigemptyset(&action.sa_mask);
        action.sa_handler = empty_handler;
        sigaction(SIGUSR1, &action, NULL);

        if (no_threads < 1) {
            fprintf(stderr, "Wrong number of threads!\n");
            exit(EXIT_FAILURE);
        }
        if (no_threads > size) {
            no_threads = size;
        }

        threads = malloc(sizeof(pthread_t) * no_threads);

        int block_size = size / no_threads;
        int j = 0;

        for(int i = 0; i < no_threads; i++) {
            UpdateArgs* args = malloc(sizeof(UpdateArgs));
            args->src = src;
            args->dst = dst;
            args->start_idx = j;
            if (i == no_threads - 1) {
                args->block_size = size - j;
            }
            else {
                args->block_size = block_size;
            }
            j += block_size;

            pthread_create(threads + i, NULL, single_update, (void*) args);
        }
    }
    
    for(int i = 0; i < no_threads; i++) {
        pthread_kill(threads[i], SIGUSR1);
    }
}