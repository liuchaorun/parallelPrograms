#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main()
{
    int my_rank, comm_sz;
    int *nums, num, ring_pass_sum, butterfly_structured_sum;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    srand(my_rank);
    // num = rand() % 1000000;
    num = my_rank;
    nums = (int *)malloc(comm_sz * sizeof(int));
    MPI_Gather(&num, 1, MPI_INT, nums, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (my_rank == 0)
    {
        printf("The numbers are: ");
        for (int i = 0; i < comm_sz; i++)
        {
            printf("%d ", nums[i]);
        }
        printf("\n");
    }

    int source = (my_rank + comm_sz - 1) % comm_sz;
    int dest = (my_rank + 1) % comm_sz;
    int tmp_val;

    ring_pass_sum = tmp_val = num;
    for (int i = 1; i < comm_sz; i++)
    {
        MPI_Sendrecv_replace(&tmp_val, 1, MPI_INT, dest, 0, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        ring_pass_sum += tmp_val;
    }

    MPI_Finalize();
    free(num);

    return 0;
}