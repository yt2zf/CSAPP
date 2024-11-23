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
    int i, j, tmp, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;
    int i1, j1;

    if (M == 32 && N == 32)
    {
        // 8*8 block, i stand for the row index of "BLOCK", j stand for the column index of "BLOCK"
        for (i = 0; i < N; i += 8)
        {
            // transpose diagonal block
            for (i1 = 0; i1 < 8; i1++)
            {
                // 先按行读A，写入B的行，每行，A读miss一次，B写miss一次
                tmp = A[i + i1][i];
                tmp1 = A[i + i1][i + 1];
                tmp2 = A[i + i1][i + 2];
                tmp3 = A[i + i1][i + 3];
                tmp4 = A[i + i1][i + 4];
                tmp5 = A[i + i1][i + 5];
                tmp6 = A[i + i1][i + 6];
                tmp7 = A[i + i1][i + 7];

                B[i + i1][i] = tmp;
                B[i + i1][i + 1] = tmp1;
                B[i + i1][i + 2] = tmp2;
                B[i + i1][i + 3] = tmp3;
                B[i + i1][i + 4] = tmp4;
                B[i + i1][i + 5] = tmp5;
                B[i + i1][i + 6] = tmp6;
                B[i + i1][i + 7] = tmp7;
            }

            // 直接对B进行转置，因为B已经在cache里了，所以对B进行读写每次都是命中
            for (i1 = 0; i1 < 8; i1++)
            {
                for (j1 = i1 + 1; j1 < 8; j1++)
                {
                    // 对角线不用处理
                    tmp = B[i + i1][i + j1];
                    B[i + i1][i + j1] = B[i + j1][i + i1];
                    B[i + j1][i + i1] = tmp;
                }
            }
            for (j = 0; j < M; j += 8)
            {
                if (i != j)
                {
                    // transpose off-diagonal block
                    for (i1 = 0; i1 < 8; i1++)
                    {
                        for (j1 = 0; j1 < 8; j1++)
                        {
                            tmp = A[i + i1][j + j1];
                            B[j + j1][i + i1] = tmp;
                        }
                    }
                }
            }
        }
        return;
    }
    else if (M == 64 && N == 64)
    {
        // 8*8 block, i stand for the row index of "BLOCK", j stand for the column index of "BLOCK"
        for (i = 0; i < N; i += 8)
        {
            for (j = 0; j < M; j += 8)
            {
                // 先读A的上半部分 (4 * 8)
                for (i1 = 0; i1 < 4; i1++)
                {   
                    tmp = A[i+i1][j];
                    tmp1= A[i+i1][j+1];
                    tmp2= A[i+i1][j+2];
                    tmp3= A[i+i1][j+3];
                    tmp4= A[i+i1][j+4];
                    tmp5= A[i+i1][j+5];
                    tmp6= A[i+i1][j+6];
                    tmp7= A[i+i1][j+7];

                    B[j][i + i1] = tmp;
                    B[j+1][i + i1] = tmp1;
                    B[j+2][i + i1] = tmp2;
                    B[j+3][i + i1] = tmp3;
                    B[j][i+i1+4] = tmp4;
                    B[j+1][i+i1+4] = tmp5;
                    B[j+2][i+i1+4] = tmp6;
                    B[j+3][i+i1+4] = tmp7;
                }

                // 处理A的右下半部分 (4 * 4)
                for (j1 = 0; j1 < 4; j1++){
                    // 变量缓存A的右下部分
                    tmp4 = A[i+4][j+j1];
                    tmp5 = A[i+5][j+j1];
                    tmp6 = A[i+6][j+j1];
                    tmp7 = A[i+7][j+j1];

                    // 变量缓存B的右上部分
                    tmp = B[j+j1][i+4];
                    tmp1 = B[j+j1][i+5];
                    tmp2 = B[j+j1][i+6];
                    tmp3 = B[j+j1][i+7];
                    
                    // 写入B的右上部分
                    B[j+j1][i+4] = tmp4;
                    B[j+j1][i+5] = tmp5;
                    B[j+j1][i+6] = tmp6;
                    B[j+j1][i+7] = tmp7;

                    // 写入B的左下部分
                    B[j+j1+4][i] = tmp;
                    B[j+j1+4][i+1] = tmp1;
                    B[j+j1+4][i+2] = tmp2;
                    B[j+j1+4][i+3] = tmp3;
                }

                // 处理A的左下半部分
                for (i1=4; i1<8;i1++){
                    tmp = A[i+i1][j+4];
                    tmp1 = A[i+i1][j+5];
                    tmp2 = A[i+i1][j+6];
                    tmp3 = A[i+i1][j+7];

                    B[j+i1][i+4] = tmp;
                    B[j+i1][i+5] = tmp1;
                    B[j+i1][i+6] = tmp2;
                    B[j+i1][i+7] = tmp3;
                }

                // 转置B的左下半部分
                for (i1 = 4; i1 < 8; i1++){
                    for (j1=i1+1; j1 < 8; j1++){
                        tmp = B[j+i1][i+j1];
                        B[j+i1][i+j1] = B[j+j1][i+i1];
                        B[j+j1][i+i1] = tmp;
                    }
                }
            }
        }
        return;
    }
    else if (M == 61 && N == 67)
    {
        // 8*8 block, i stand for the row index of "BLOCK", j stand for the column index of "BLOCK"
        for (i = 0; i < 64; i += 8)
        {
            for (j = 0; j < 56; j += 8)
            {
                // 先读A，写入B的行， A read miss 1 and B write miss 1 per line
                for (i1 = 0; i1 < 8; i1++)
                {
                    tmp = A[i + i1][j];
                    tmp1 = A[i + i1][j + 1];
                    tmp2 = A[i + i1][j + 2];
                    tmp3 = A[i + i1][j + 3];
                    tmp4 = A[i + i1][j + 4];
                    tmp5 = A[i + i1][j + 5];
                    tmp6 = A[i + i1][j + 6];
                    tmp7 = A[i + i1][j + 7];

                    // 注意A的block(i, j) 要放到B的block(j, i)
                    B[j + i1][i] = tmp;
                    B[j + i1][i + 1] = tmp1;
                    B[j + i1][i + 2] = tmp2;
                    B[j + i1][i + 3] = tmp3;
                    B[j + i1][i + 4] = tmp4;
                    B[j + i1][i + 5] = tmp5;
                    B[j + i1][i + 6] = tmp6;
                    B[j + i1][i + 7] = tmp7;
                }
                // 直接对B进行转置，因为B已经在cache里了，所以对B进行读写每次都是命中
                for (i1 = 0; i1 < 8; i1++)
                {
                    for (j1 = i1 + 1; j1 < 8; j1++)
                    {
                        // 对角线不用处理
                        tmp = B[j + i1][i + j1];
                        B[j + i1][i + j1] = B[j + j1][i + i1];
                        B[j + j1][i + i1] = tmp;
                    }
                }
            }
        }

        // 3*8 block --> 67 * 56 done
        for (i = 64; i < 67; i += 3)
        {
            for (j = 0; j < 56; j += 8)
            {
                for (i1 = 0; i1 < 3; i1++)
                {
                    tmp = A[i + i1][j];
                    tmp1 = A[i + i1][j + 1];
                    tmp2 = A[i + i1][j + 2];
                    tmp3 = A[i + i1][j + 3];
                    tmp4 = A[i + i1][j + 4];
                    tmp5 = A[i + i1][j + 5];
                    tmp6 = A[i + i1][j + 6];
                    tmp7 = A[i + i1][j + 7];

                    // 注意A的block(i, j) 要放到B的block(j, i)
                    B[j + i1][i] = tmp;
                    B[j + i1][i + 1] = tmp1;
                    B[j + i1][i + 2] = tmp2;
                    B[j + i1][i + 3] = tmp3;
                    B[j + i1][i + 4] = tmp4;
                    B[j + i1][i + 5] = tmp5;
                    B[j + i1][i + 6] = tmp6;
                    B[j + i1][i + 7] = tmp7;
                }

                // 直接对B进行转置，因为B已经在cache里了，所以对B进行读写每次都是命中
                for (i1 = 0; i1 < 3; i1++)
                {
                    for (j1 = i1 + 1; j1 < 8; j1++)
                    {
                        // 对角线不用处理
                        tmp = B[j + i1][i + j1];
                        B[j + i1][i + j1] = B[j + j1][i + i1];
                        B[j + j1][i + i1] = tmp;
                    }
                }
            }
        }

        // 8*4 block  --> 64 * 60 done
        for (i = 0; i < 64; i += 8)
        {
            for (j = 56; j < 60; j += 4)
            {
                for (i1 = 0; i1 < 8; i1++)
                {
                    tmp = A[i + i1][j];
                    tmp1 = A[i + i1][j + 1];
                    tmp2 = A[i + i1][j + 2];
                    tmp3 = A[i + i1][j + 3];

                    B[j + i1][i] = tmp;
                    B[j + i1][i + 1] = tmp1;
                    B[j + i1][i + 2] = tmp2;
                    B[j + i1][i + 3] = tmp3;
                }
            }
            for (i1 = 0; i1 < 8; i1++)
            {
                for (j1 = i1 + 1; j1 < 4; j1++)
                {
                    // 对角线不用处理
                    tmp = B[j + i1][i + j1];
                    B[j + i1][i + j1] = B[j + j1][i + i1];
                    B[j + j1][i + i1] = tmp;
                }
            }
        }

        // 3 * 4 block  --> 67 * 60 done
        for (i = 64; i < 67; i += 3)
        {
            for (j = 56; j < 60; j += 4){
                for (i1 = 0; i1 < 3; i1++)
                {
                    tmp = A[i + i1][j];
                    tmp1 = A[i + i1][j + 1];
                    tmp2 = A[i + i1][j + 2];
                    tmp3 = A[i + i1][j + 3];

                    B[j + i1][i] = tmp;
                    B[j + i1][i + 1] = tmp1;
                    B[j + i1][i + 2] = tmp2;
                    B[j + i1][i + 3] = tmp3;
                }
            }
            for (i1 = 0; i1 < 3; i1++)
            {
                for (j1 = i1 + 1; j1 < 4; j1++)
                {
                    // 对角线不用处理
                    tmp = B[j + i1][i + j1];
                    B[j + i1][i + j1] = B[j + j1][i + i1];
                    B[j + j1][i + i1] = tmp;
                }
            }
        }

        // last column of A, transpose to last row of B
        for (i = 0; i < 67; i++)
        {
            tmp = A[i][60];
            B[60][i] = tmp;
        }

        return;
    }

    for (i = 0; i < N; i++)
    {
        for (j = 0; j < M; j++)
        {
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
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < M; j++)
        {
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

    for (i = 0; i < N; i++)
    {
        for (j = 0; j < M; ++j)
        {
            if (A[i][j] != B[j][i])
            {
                return 0;
            }
        }
    }
    return 1;
}
