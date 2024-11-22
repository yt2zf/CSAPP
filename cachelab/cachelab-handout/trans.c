/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;
    int blockSizeRow;
    int blockSizeCol ;
    int i1, j1;

    if (M == 32){
        blockSizeRow = 8;
        blockSizeCol = 8;
    } else if (M == 64){
        blockSizeRow = 64;
        blockSizeCol = 8;

        for (i = 0; i < N; i += blockSizeRow)
        {
            for (j = 0; j < M; j += blockSizeCol)
            {
                // block transpose

                // A的64*4
                for (int rowA = 0; rowA < 64; rowA++)
                {
                    // A column的前4列，B column的前4行
                    for (int colOffset = 0; colOffset < 4; colOffset++)
                    {
                        if (rowA / 8 == j / 8 && (colOffset == rowA % 4))
                        {
                            continue;
                        }
                        tmp = A[rowA][j + colOffset];
                        B[j + colOffset][rowA] = tmp;
                    }
                    if (rowA / 8 == j / 8)
                    {
                        tmp = A[rowA][j + rowA % 4];
                        B[j + rowA % 4][rowA] = tmp;
                    }
                }

                // A的next 64*4
                for (int rowA = 0; rowA < 64; rowA++)
                {
                    // A column的前4列，B column的前4行
                    for (int colOffset = 0; colOffset < 4; colOffset++)
                    {
                        if (rowA / 8 == j / 8 && (colOffset == rowA % 4))
                        {
                            continue;
                        }
                        tmp = A[rowA][j + colOffset + 4];
                        B[j + colOffset + 4][rowA] = tmp;
                    }
                    if (rowA / 8 == j / 8)
                    {
                        tmp = A[rowA][j + rowA % 4 + 4];
                        B[j + rowA % 4 + 4][rowA] = tmp;
                    }
                }
            }
        }
        return;
    } else{
        blockSizeRow = 1;
        blockSizeCol = 1;
    }

    for (i = 0; i < N; i += blockSizeRow) {
        for (j = 0; j < M; j += blockSizeCol) {
            // block transpose
            for (i1 = 0; i1 < blockSizeRow; i1++){
                for (j1 = 0; j1 < blockSizeCol; j1++){
                    if (i1 != j1){
                        // skip the diagonal of BLOCK first
                        tmp = A[i + i1][j + j1];
                        B[j + j1][i + i1] = tmp;
                    }
                }

                // Do transpose for the diagonal of BLOCK
                tmp = A[i + i1][j + i1];
                B[j + i1][i + i1] = tmp;
            }
        }
    }
    
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;
    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc); 

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

