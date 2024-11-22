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

void transpose_diagonal_block32(int blocki){

}

void transpose_offdiagonal_block32(int blocki, int blockj){
}

void transpose_offdiagonal_block64(int blocki, int blockj){

}
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
    int i, j, tmp, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;
    int i1, j1;

    if (M == 32){
       for (i=0; i<4; i++){
            // transpose diagonal block
            for (i1 = 0; i1 <8; i1++){
                // 先按行读A，写入B的行，每行，A读miss一次，B写miss一次
                tmp = A[i*8 + i1][i*8];
                tmp1= A[i*8 + i1][i*8+1];
                tmp2= A[i*8 + i1][i*8+2];
                tmp3= A[i*8 + i1][i*8+3];
                tmp4= A[i*8 + i1][i*8+4];
                tmp5= A[i*8 + i1][i*8+5];
                tmp6= A[i*8 + i1][i*8+6];
                tmp7= A[i*8 + i1][i*8+7];

                B[i*8 + i1][i*8] = tmp;
                B[i*8 + i1][i*8+1] = tmp1;
                B[i*8 + i1][i*8+2] = tmp2;
                B[i*8 + i1][i*8+3] = tmp3;
                B[i*8 + i1][i*8+4] = tmp4;
                B[i*8 + i1][i*8+5] = tmp5;
                B[i*8 + i1][i*8+6] = tmp6;
                B[i*8 + i1][i*8+7] = tmp7;
            }

            // 直接对B进行转置，因为B已经在cache里了，所以对B进行读写每次都是命中
            for (i1 = 0; i1 < 8; i1++){
                for (j1 = i1+1; j1<8;j1++){
                    // 对角线不用处理
                    tmp =  B[i*8 + i1][i*8 + j1];
                    B[i*8 + i1][i*8 + j1] = B[i*8 + j1][i*8 + i1];
                    B[i*8 + j1][i*8 + i1] = tmp;
                    
                }
            }
            for (j=0; j<4;j++){
                if (i != j){
                    // transpose off-diagonal block
                    for (i1 = 0; i1<8; i1++){
                        for (j1=0; j1<8; j1++){
                            tmp = A[i * 8 + i1][j*8 + j1];
                            B[j*8 + j1][i * 8 + i1] = tmp;
                        }
                    }
                }
            }
       }
       return;
    } else if (M == 64){
        for (i = 0; i < N; i += 64)
        {
            for (j = 0; j < M; j += 8)
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
    }

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
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

