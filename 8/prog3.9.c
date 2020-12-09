/**
 * problem 3.9
 * Write an MPI program that implements multiplication of a vector by a scalar and dot product. 
 * The user should enter two vectors and a scalar, all of which are read in by process 0 and distributed among the processes. 
 * The results are calculated and collected onto process 0, which prints them. 
 * You can assume that n, the order of the vectors, is evenly divisible by comm_sz.
 */
#include <stdio.h>
#include <malloc.h>
#include <mpi.h>

int main() {
    int my_rank;
    int comm_sz;
    int n;
    double scalar;
    double *vector1, *vector2, *local_vector1, *local_vector2, *local_result, *result;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    //input data
    if (my_rank == 0) {
        printf("Please input the order of the vectors:\n");
        scanf("%d", &n);
        MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
        vector1 = (double *)malloc(n * sizeof(double));
        vector2 = (double *)malloc(n * sizeof(double));
        result = (double *)malloc(n * sizeof(double));
        printf("Please input the scalar:\n");
        scanf("%lf", &scalar);
        MPI_Bcast(&scalar, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        printf("Please input the vector1:\n");
        for (int i = 0; i < n; i++) {
            scanf("%lf", &vector1[i]);
        }
        printf("Please input the vector2:\n");
        for (int i = 0; i < n; i++) {
            scanf("%lf", &vector2[i]);
        }
        if (n % comm_sz != 0) {
            return 0;
        }
        local_vector1 = (double *)malloc(n / comm_sz * sizeof(double));
        local_vector2 = (double *)malloc(n / comm_sz * sizeof(double));
        MPI_Scatter(vector1, n/comm_sz, MPI_DOUBLE, local_vector1, n/comm_sz, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Scatter(vector2, n/comm_sz, MPI_DOUBLE, local_vector2, n/comm_sz, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    } else {
        MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&scalar, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        vector1 = (double *)malloc(n * sizeof(double));
        vector2 = (double *)malloc(n * sizeof(double));
        result = (double *)malloc(n * sizeof(double));
        if (n % comm_sz != 0) {
            return 0;
        }
        local_vector1 = (double *)malloc(n / comm_sz * sizeof(double));
        local_vector2 = (double *)malloc(n / comm_sz * sizeof(double));
        MPI_Scatter(vector1, n/comm_sz, MPI_DOUBLE, local_vector1, n/comm_sz, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Scatter(vector2, n/comm_sz, MPI_DOUBLE, local_vector2, n/comm_sz, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    }
    
    local_result = (double *)malloc(n / comm_sz * sizeof(double));
    for (int i = 0; i < n/comm_sz; i++) {
        local_result[i] = local_vector1[i] * scalar;
    }
    MPI_Gather(local_result, n / comm_sz, MPI_DOUBLE, result, n / comm_sz, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    if (my_rank == 0) {
        printf("The first vector is: ");
        for (int i = 0; i < n; i++) {
            printf("%lf ", vector1[i]);
        }
        printf("\n");
        printf("The second vector is: ");
        for (int i = 0; i < n; i++) {
            printf("%lf ", vector2[i]);
        }
        printf("\n");
        printf("The scalar is: %f \n", scalar);
        printf("The result of the first vector by a scalar is: ");
        for (int i = 0; i < n; i++) {
            printf("%lf ", result[i]);
        }
        printf("\n");
    }

    local_result = (double *)malloc(n / comm_sz * sizeof(double));
    for (int i = 0; i < n/comm_sz; i++) {
        local_result[i] = local_vector2[i] * scalar;
    }
    MPI_Gather(local_result, n / comm_sz, MPI_DOUBLE, result, n / comm_sz, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    if (my_rank == 0) {
        printf("The result of the second vector by a scalar is: ");
        for (int i = 0; i < n; i++) {
            printf("%lf ", result[i]);
        }
        printf("\n");
    }

    local_result = (double *)malloc(n / comm_sz * sizeof(double));
    for (int i = 0; i < n/comm_sz; i++) {
        local_result[i] = local_vector2[i] * local_vector1[i];
    }
    MPI_Gather(local_result, n / comm_sz, MPI_DOUBLE, result, n / comm_sz, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    if (my_rank == 0) {
        printf("The result of the first vector dot the second vector is : ");
        for (int i = 0; i < n; i++) {
            printf("%lf ", result[i]);
        }
        printf("\n");
    }

    free(vector1);
    free(vector2);
    free(result);
    free(local_vector1);
    free(local_vector2);
    free(local_result);
    MPI_Finalize();
    return 0;
}