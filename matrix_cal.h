#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define ROW 1024
#define COL 1024

#define MIN_TILE 16
#define MAX_TILE 1024

#define FLUSH_AMT ROW*COL

/* verbose == 1: print the whole matrix; == 2: print [1023][1023] for simple checking; == 3 print stride followed by time elapsed; == 0: only print run time information */
#define VERBOSE 0

void initial_matrix(int **A, int row, int col, char* name);
void print_matrix(int **A, int row, int col, char* name);
clock_t matrix_multiplication(int **C, int **A, int **B, int stride);
clock_t matrix_mult_uneven(int **C, int **A, int **B, int stride);
void clear_matrix(int **C);
