#include <stdio.h>
#include <mpi.h>
#include <malloc.h>

int main()
{
    int my_rank, comm_sz, order;
    double *matrix;
    MPI_Datatype trangular_data_type;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    if (comm_sz < 2) {
        printf("This prgram need two proccesses.");
        return 0;
    }

    if (my_rank == 0)
    {
        printf("Please input the order of matrix: ");
        scanf("%d", &order);
        MPI_Bcast(&order, 1, MPI_INT, 0, MPI_COMM_WORLD);
        matrix = (double *)malloc(order * order * sizeof(double));
        printf("please input the data of matrix: \n");
        for (int i = 0; i < order; i++) 
        {
            for (int j = 0; j < order; j++)
            {
                scanf("%lf", &matrix[j + i * order]);
            }
            
        }
        int *array_of_block_lengths, *array_of_displacements;
        array_of_block_lengths = (int *)malloc(order * sizeof(int));
        array_of_displacements = (int *)malloc(order * sizeof(int));
        for (int i = 0; i < order; i++) {
            array_of_block_lengths[i] = order - i;
            array_of_displacements[i] = i * (order + 1);
        }
        MPI_Type_indexed(order, array_of_block_lengths, array_of_displacements, MPI_DOUBLE, &trangular_data_type);
        MPI_Type_commit(&trangular_data_type);
        free(array_of_block_lengths);
        free(array_of_displacements);
        MPI_Send(matrix, 1, trangular_data_type, 1, 0, MPI_COMM_WORLD);
    }
    else if (my_rank == 1)
    {
        MPI_Bcast(&order, 1, MPI_INT, 0, MPI_COMM_WORLD);
        int *array_of_block_lengths, *array_of_displacements;
        array_of_block_lengths = (int *)malloc(order * sizeof(int));
        array_of_displacements = (int *)malloc(order * sizeof(int));
        for (int i = 0; i < order; i++) {
            array_of_block_lengths[i] = order - i;
            array_of_displacements[i] = i * (order + 1);
        }
        MPI_Type_indexed(order, array_of_block_lengths, array_of_displacements, MPI_DOUBLE, &trangular_data_type);
        MPI_Type_commit(&trangular_data_type);
        free(array_of_block_lengths);
        free(array_of_displacements);
        matrix = (double *)malloc(order * order * sizeof(double));
        for (int i = 0; i < order; i++) 
        {
            for (int j = 0; j < order; j++)
            {
                matrix[j + i * order] = 0;
            }
            
        }
        MPI_Recv(matrix, 1, trangular_data_type, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    if (my_rank == 0) {
        printf("The mtrix in proccess 0 is: \n");
        for (int i = 0; i < order; i++) 
        {
            for (int j = 0; j < order; j++)
            {
                printf("%lf\t", matrix[j + i * order]);
            }
            printf("\n");
        }
        printf("\n");
    }
    if (my_rank == 1) {
        printf("The mtrix in proccess 1 is: \n");
        for (int i = 0; i < order; i++) 
        {
            for (int j = 0; j < order; j++)
            {
                printf("%lf\t", matrix[j + i * order]);
            }
            printf("\n");
        }
        printf("\n");
    }

    MPI_Finalize();
    free(matrix);
    return 0;
}