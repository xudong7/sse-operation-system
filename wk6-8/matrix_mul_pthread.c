/**
 * Matrix (N*N) multiplication with multiple threads.
 * 完成矩阵乘法运算，拍照已补全的worker函数部分的代码
 * 按照矩阵大小为2000，线程数分别为1到 8，各运行5次，求各线程数对应的平均运行时间
 * 画出实际计算的加速比曲线，并与理想加速比曲线做比较，拍照上传实际加速比曲线（可以用excel中的画图工具完成）
 * 尝试解释实际加速比和理想加速比存在差异的原因
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>

int size, num_threads;
double **matrix1, **matrix2, **matrix3;

double **allocate_matrix(int size)
{
    /* Allocate 'size' * 'size' doubles contiguously. */
    double *vals = (double *)malloc(size * size * sizeof(double));

    /* Allocate array of double* with size 'size' */
    double **ptrs = (double **)malloc(size * sizeof(double *));

    int i;
    for (i = 0; i < size; ++i)
    {
        ptrs[i] = &vals[i * size];
    }

    return ptrs;
}

void init_matrix(double **matrix, int size)
{
    int i, j;

    for (i = 0; i < size; ++i)
    {
        for (j = 0; j < size; ++j)
        {
            matrix[i][j] = 1.0;
        }
    }
}

void print_matrix(double **matrix, int size)
{
    int i, j;

    for (i = 0; i < size; ++i)
    {
        for (j = 0; j < size - 1; ++j)
        {
            printf("%lf, ", matrix[i][j]);
        }
        printf("%lf", matrix[i][j]);
        putchar('\n');
    }
}

/**
 * Thread routine.
 * Each thread works on a portion of the 'matrix1'.
 * The start and end of the portion depend on the 'arg' which
 * is the ID assigned to threads sequentially.
 */
void *worker(void *arg)
{
    // printf("Thread %d started\n", *(int *)arg);
    int i, j, k;
    int start = (*(int *)arg) * (size / num_threads);
    int end = (*(int *)arg + 1) * (size / num_threads);
    if (*(int *)arg == num_threads - 1)
    {
        end = size;
    }
    for (i = start; i < end; ++i)
    {
        for (j = 0; j < size; ++j)
        {
            matrix3[i][j] = 0;
            for (k = 0; k < size; ++k)
            {
                matrix3[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }
    // printf("Thread %d finished\n", *(int *)arg);
    free(arg); 
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    int i;
    double sum = 0;
    struct timeval tstart, tend;
    double exectime;
    pthread_t *threads;

    if (argc != 3)
    {
        fprintf(stderr, "%s <matrix size> <number of threads>\n", argv[0], argv[1]);
        return -1;
    }

    size = atoi(argv[1]);
    num_threads = atoi(argv[2]);

    threads = (pthread_t *)malloc(num_threads * sizeof(pthread_t));

    matrix1 = allocate_matrix(size);
    matrix2 = allocate_matrix(size);
    matrix3 = allocate_matrix(size);

    init_matrix(matrix1, size);
    init_matrix(matrix2, size);

    if (size <= 10)
    {
        printf("Matrix 1:\n");
        print_matrix(matrix1, size);
        printf("Matrix 2:\n");
        print_matrix(matrix2, size);
    }

    gettimeofday(&tstart, NULL);
    for (i = 0; i < num_threads; ++i)
    {
        int *tid;
        tid = (int *)malloc(sizeof(int));
        *tid = i;
        pthread_create(&threads[i], NULL, worker, (void *)tid);
    }

    for (i = 0; i < num_threads; ++i)
    {
        pthread_join(threads[i], NULL);
    }
    gettimeofday(&tend, NULL);

    if (size <= 10)
    {
        printf("Matrix 3:\n");
        print_matrix(matrix3, size);
    }

    exectime = (tend.tv_sec - tstart.tv_sec) * 1000.0;    // sec to ms
    exectime += (tend.tv_usec - tstart.tv_usec) / 1000.0; // us to ms

    printf("Matrix size: %dx%d, Number of threads: %d\tExecution time:%.3lf sec\n",
           size, size, num_threads, exectime / 1000.0);

    return 0;
}