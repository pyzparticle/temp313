#include "matrix_cal.h"

// Clear matrix elements to 0
//---------------------
void clear_matrix(int ** C)
{
        int i = 0;
        int j = 0;
        if(VERBOSE == 1) printf("Initialize C to all 0. \n\n");
        for(i = 0; i < ROW; i++) {
                if(VERBOSE == 1) printf("Initialize matrix C line i = %d. \n\n", i);
                for(j = 0; j < COL; j++) {
                        C[i][j] = 0;
                }
        }
}

// C = A * B
//-----------
clock_t matrix_multiplication(int **C, int **A, int **B, int stride)
{
        int i,j,k,l,m,n,sum;

        clock_t CPU_start_time = clock();

        if(VERBOSE == 2) {
                printf("(Stride = %d) CPU start time is: %lu \n", stride, CPU_start_time);
        }

        for(i = 0; i < ROW; i = i + stride ) {
                for(j = 0; j < COL; j = j + stride) {
                        for(k = 0; k < COL; k = k + stride) {
                                // calculation for one tile
                                for(l = i; l < i + stride; l++) {
                                        for(m = j; m < j + stride; m++) {
                                                sum = 0;
                                                for(n = k; n < k+stride; n++) {
                                                        sum = sum + A[l][n] * B[n][m];
                                                }
                                                C[l][m] += sum;
                                        }
                                }
                        }
                }
        }

        clock_t CPU_end_time = clock();

        if(VERBOSE == 2) {
                printf("(Stride = %d) CPU end time is: %lu \n", stride, CPU_end_time);
        }

        if(VERBOSE == 3) {
                printf("(Stride = %5d) CPU run time is: %10lu \n\n", stride, CPU_end_time - CPU_start_time);
        }

        if(VERBOSE == 1) {
                print_matrix(C, ROW, COL, "C");
        }

        if(VERBOSE == 4) {
                print_matrix(C, ROW, COL, "C");
        }

        return CPU_end_time - CPU_start_time;
}

// initialize matrix with random data
//-----------------------------------
void initial_matrix(int **A, int row, int col, char* name){
        int i = 0;
        int j = 0;
        if(VERBOSE == 1) printf("\nInitialize matrix %s with random data. \n\n", name);
        for(i = 0; i < row; i++) {
                if(VERBOSE == 1) printf("Intialize line i = %d for matrix %s \n", i, name);
                for(j = 0; j < col; j++) {
                        //A[i][j] = (rand() & 0xFF) + 1;
                        A[i][j] = (rand() & 0xF) + 1;
                }
        }
}

// print the matrix for checking the value
//----------------------------------------
void print_matrix(int *A[COL], int row, int col, char* name){
        int i = 0;
        int j = 0;
        for(i = 0; i < row; i++) {
                printf("\n%s[%d] = ",name, i);
                for(j = 0; j < col; j++) {
                        printf("%8d", A[i][j]);
                }
        }
        printf("\n\n");
}

// C = A * B
//-----------
clock_t matrix_transpose_multi(int **C, int **A, int **B, int stride)
{
        int i,j,k,l,m,n,sum,ti,tj,tk;
        int **submat = malloc(stride * sizeof(int *) + stride * stride * sizeof(int));
        int *off = &submat[stride]; 
        for(i = 0; i < stride; i++, off+=stride){
                submat[i] = off;
        }

        clock_t CPU_start_time = clock();

        if(VERBOSE == 2) {
                printf("(Stride = %d) CPU start time is: %lu \n", stride, CPU_start_time);
        }

        for(i = 0; i < ROW; i = i + stride ) {
                for(j = 0; j < COL; j = j + stride) {
                        for(k = 0; k < COL; k = k + stride) {
                                // calculation for one tile
                                for(tj = j; tj < j + stride; tj++)
                                        for(tk = k; tk < k + stride; tk++)
                                                submat[tj-j][tk-k] = B[tk][tj];
                                for(l = i; l < i + stride; l++) {
                                        for(m = j; m < j + stride; m++) {
                                                sum = 0;
                                                for(n = k; n < k+stride; n++) {
                                                        sum = sum + A[l][n] * submat[m-j][n-k];
                                                }
                                                C[l][m] += sum;
                                        }
                                }
                        }
                }
        }

        clock_t CPU_end_time = clock();

        if(VERBOSE == 2) {
                printf("(Stride = %d) CPU end time is: %lu \n", stride, CPU_end_time);
        }

        if(VERBOSE == 3) {
                printf("(Stride = %5d) CPU run time is: %10lu \n\n", stride, CPU_end_time - CPU_start_time);
        }

        if(VERBOSE == 2) {
                printf("matrix[%d][%d] = %d (To make sure the result is the same with different stride)\n\n", ROW, COL, C[ROW-1][COL-1]);
        }

        if(VERBOSE == 4) {
                print_matrix(C, ROW, COL, "C");
        }

        return CPU_end_time - CPU_start_time;
}

clock_t matrix_mult_uneven(int **C, int **A, int **B, int stride)
{
        int i,j,k,l,m,n,sum;

        clock_t CPU_start_time = clock();

        if(VERBOSE == 2) {
                printf("(Stride = %d) CPU start time is: %lu \n", stride, CPU_start_time);
        }

        for(i = 0; i < ROW; i = i + stride) {
                for(j = 0; j < COL; j = j + stride) {
                        for(k = 0; k < COL; k = k + stride) {
                                // calculation for one tile
                                for(l = i; l < i + (((i+stride)<ROW) ? stride : (ROW-i)); l++) {
                                        for(m = j; m < j + (((j+stride)<COL) ? stride : (COL-j)); m++) {
                                                sum = 0;
                                                for(n = k; n < k + (((k+stride)<COL) ? stride : (COL-k)); n++) {
                                                        sum = sum + A[l][n] * B[n][m];
                                                }
                                                C[l][m] += sum;
                                        }
                                }
                        }
                }
        }

        clock_t CPU_end_time = clock();

        if(VERBOSE == 2) {
                printf("(Stride = %d) CPU end time is: %lu \n", stride, CPU_end_time);
        }

        if(VERBOSE == 3) {
                printf("(Stride = %5d) CPU run time is: %10lu \n\n", stride, CPU_end_time - CPU_start_time);
        }

        if(VERBOSE == 4) {
                print_matrix(C, ROW, COL, "C");
        }

        return CPU_end_time - CPU_start_time;

}
