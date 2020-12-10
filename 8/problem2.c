#include <stdio.h>
#include <malloc.h>
#include <time.h>
#include <stdlib.h>
#include <mpi.h>

#define INDEX(x, y, n) (x * n + y)

int main()
{
    int my_rank;
    int comm_sz;
    double *vector, *local_matrix, *local_result, *result;
    double start, end, used_time, parallel_time;
    int n;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    if (my_rank == 0)
    {
        printf("please input the order of matrix and vector:\n");
        scanf("%d", &n);
        MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
        vector = (double *)malloc(n * sizeof(double));
        srand((unsigned)time(NULL));
        for (int i = 0; i < n; i++)
        {
            vector[i] = rand() / ((double)(RAND_MAX) / 1000);
        }
        if (n % comm_sz == 0)
        {
            local_matrix = (double *)malloc(n * (n / comm_sz) * sizeof(double));
            for (int i = 0; i < n / comm_sz; i++)
            {
                for (int j = 0; j < n; j++)
                {
                    local_matrix[INDEX(i, j, n)] = rand() / ((double)(RAND_MAX) / 1000);
                }
            }
        }
        else
        {
            printf("error!");
            return 0;
        }
        MPI_Bcast(vector, n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    }
    else
    {
        MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
        vector = (double *)malloc(n * sizeof(double));
        MPI_Bcast(vector, n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        if (n % comm_sz == 0)
        {
            local_matrix = (double *)malloc(n * (n / comm_sz) * sizeof(double));
            srand((unsigned)time(NULL));
            for (int i = 0; i < n / comm_sz; i++)
            {
                for (int j = 0; j < n; j++)
                {
                    local_matrix[INDEX(i, j, n)] = rand() / ((double)(RAND_MAX) / 1000);
                }
            }
        }
        else
        {
            printf("error!");
            return 0;
        }
    }
    local_result = (double *)malloc(n / comm_sz * sizeof(double));
    result = (double *)malloc(n * sizeof(double));
    int x_length = n / comm_sz;

    MPI_Barrier(MPI_COMM_WORLD);
    start = MPI_Wtime();
    for (int i = 0; i < x_length; i++)
    {
        local_result[i] = 0.0;
        for (int j = 0; j < n; j++)
        {
            local_result[i] += local_matrix[INDEX(i, j, n)] * vector[j];
        }
    }
    MPI_Gather(local_result, x_length, MPI_DOUBLE, result, x_length, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    end = MPI_Wtime();
    used_time = end - start;
    MPI_Reduce(&used_time, &parallel_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if (my_rank == 0)
    {
        printf("the used time is: %lf\n", parallel_time);
    }

    MPI_Finalize();
    free(vector);
    free(local_matrix);
    free(local_result);
    free(result);
}