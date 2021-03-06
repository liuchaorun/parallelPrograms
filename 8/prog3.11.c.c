#include <stdio.h>
#include <malloc.h>
#include <mpi.h>

int main()
{
    double *vector, *prefixSums;
    int n;
    int my_rank;
    int comm_sz;
    double *localVector, *localPrexfixSums;
    double bais;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    if (my_rank == 0)
    {
        printf("Please input the order of the vectors:\n");
        scanf("%d", &n);
        MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
        if (n % comm_sz != 0)
            return 0;
        vector = (double *)malloc(n * sizeof(double));
        prefixSums = (double *)malloc(n * sizeof(double));
        printf("Please input the vector:\n");
        for (int i = 0; i < n; i++)
        {
            scanf("%lf", &vector[i]);
        }
        localVector = (double *)malloc(n / comm_sz * sizeof(double));
        MPI_Scatter(vector, n / comm_sz, MPI_DOUBLE, localVector, n / comm_sz, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    }
    else
    {
        MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
        vector = (double *)malloc(n * sizeof(double));
        prefixSums = (double *)malloc(n * sizeof(double));
        if (n % comm_sz != 0)
            return 0;
        localVector = (double *)malloc(n / comm_sz * sizeof(double));
        MPI_Scatter(vector, n / comm_sz, MPI_DOUBLE, localVector, n / comm_sz, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    }
    localPrexfixSums = (double *)malloc(n / comm_sz * sizeof(double));

    for (int i = 0; i < n / comm_sz; i++)
    {
        if (i == 0)
        {
            localPrexfixSums[0] = localVector[0];
        }
        else
        {
            localPrexfixSums[i] = localPrexfixSums[i - 1] + localVector[i];
        }
    }

    unsigned bitMask = 1;
    double localBais = localPrexfixSums[n / comm_sz - 1];
    while (bitMask < comm_sz)
    {
        int p = my_rank ^ bitMask;
        MPI_Sendrecv(&localBais, 1, MPI_DOUBLE, p, 0, &bais, 1, MPI_DOUBLE, p, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        if (my_rank > p)
        {
            for (int i = 0; i < n / comm_sz; i++)
            {

                localPrexfixSums[i] += bais;
            }
        }
        localBais += bais;
        
        bitMask <<= 1;
    }

    MPI_Gather(localPrexfixSums, n / comm_sz, MPI_DOUBLE, prefixSums, n / comm_sz, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (my_rank == 0)
    {
        printf("The result of prefix sums is: ");
        for (int i = 0; i < n; i++)
        {
            printf("%lf ", prefixSums[i]);
        }
        printf("\n");
    }

    MPI_Finalize();
    free(vector);
    free(prefixSums);
    free(localVector);
    free(localPrexfixSums);

    return 0;
}