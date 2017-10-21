#include "matrix_cal.h"

static void flush(int dim)
{
    const int size = FLUSH_AMT;
    char *c = (char *)malloc(size);
    for (int i = 0; i < 0xf; i++)
        for (int j = 0; j < size; j++)
            c[j] = i+j;
}

int main()
{
    clock_t result;
    int i, j, t_size;
    int size;

    // Allocate memory for A 1024x1024
    size = ROW * sizeof(int *) + ROW * COL * sizeof(int);
    int **A =  malloc(size);
    printf("Allocate memory for A 1024*1024 (size = %d Byte). \n", size);
    int *offsA = &A[ROW];
    for(i = 0; i < ROW; i++, offsA+= COL){
        A[i] = offsA;
    }

    // Allocate memory for B 1024 x (1024+3))
    size = ROW * sizeof(int *) + ROW * (COL+3) * sizeof(int);
    int **B =  malloc(size);
    printf("Allocate memory for B 1024*1027 (size = %d Byte). \n", size);
    int *offsB = &B[ROW];
    for(i = 0; i < ROW; i++, offsB+= (COL+3)){
        B[i] = offsB;
    }

    // Allocate memory for C 1024x(1204+5)
    size = ROW * sizeof(int *) + ROW * (COL+5) * sizeof(int);
    int **C =  malloc(ROW * sizeof(int *) + ROW * (COL+5) * sizeof(int));
    printf("Allocate memory for C 1024*1029 (size = %d Byte). \n\n", size);
    int *offsC = &C[ROW];
    for(i = 0; i < ROW; i++, offsC+= (COL+5)){
        C[i] = offsC;
    }

    // A, B assign random value
    initial_matrix(A, ROW, COL, "A");
    initial_matrix(B, ROW, COL, "B");

    for (t_size = MIN_TILE; t_size <= MAX_TILE; t_size = t_size << 1) {
        clear_matrix(C);
        flush(FLUSH_AMT);
        result =  matrix_multiplication(C, A, B, t_size);
        if (VERBOSE == 0)
            printf("%lu,", result);
    }
    printf("\n");
}
