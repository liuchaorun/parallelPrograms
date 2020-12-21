#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main()
{
    int my_rank, comm_sz;
    int *nums, num, ring_pass_sum, butterfly_structured_sum;
    double start, end, my_time, used_time;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    srand(my_rank);
    num = rand() % 100;
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

    //ring-pass
    MPI_Barrier(MPI_COMM_WORLD);
    start = MPI_Wtime();

    int source = (my_rank + comm_sz - 1) % comm_sz;
    int dest = (my_rank + 1) % comm_sz;
    int tmp_val;

    ring_pass_sum = tmp_val = num;
    for (int i = 1; i < comm_sz; i++)
    {
        MPI_Sendrecv_replace(&tmp_val, 1, MPI_INT, dest, 0, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        ring_pass_sum += tmp_val;
    }

    end = MPI_Wtime();
    my_time = end - start;
    MPI_Reduce(&my_time, &used_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    if (my_rank == 0)
    {
        printf("ring pass, the sum is %d, the used time is %lf\n", ring_pass_sum, used_time);
    }

    //butterfly-structured
    MPI_Barrier(MPI_COMM_WORLD);
    start = MPI_Wtime();
    int partner;
    int bit_mask = 1;
    int tmp_num;
    butterfly_structured_sum = num;

    while (bit_mask < comm_sz) {
        partner = my_rank ^ bit_mask;
        MPI_Sendrecv(&butterfly_structured_sum,  1, MPI_INT, partner, 0, &tmp_num, 1, MPI_INT, partner, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        butterfly_structured_sum += tmp_num;
        bit_mask <<= 1;
    }

    end = MPI_Wtime();
    my_time = end - start;
    MPI_Reduce(&my_time, &used_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    if (my_rank == 0)
    {
        printf("butterfly structured, the sum is %d, the used time is %lf\n", butterfly_structured_sum, used_time);
    }

    MPI_Finalize();
    free(nums);

    return 0;
}