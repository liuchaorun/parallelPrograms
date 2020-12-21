#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(void) {
  int comm_sz;
  int my_rank;
  long long int total_number_of_toss;
  long long int local_number_of_toss;
  long long int total_number_in_circle = 0;
  long long int local_number_in_circle = 0;

  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  //input data from proccess 0
  if (my_rank == 0) {
    printf("Please input the total number of toss:\n");
    scanf("%lld", &total_number_of_toss);
  }

  //send data to other processes
  MPI_Bcast(&total_number_of_toss, 1, MPI_LONG_LONG_INT, 0, MPI_COMM_WORLD);
  local_number_of_toss = total_number_of_toss / comm_sz;

  //caclulate
  srand(my_rank + 1);
  double x, y, distance_squared;
  for (int i = 0; i < local_number_of_toss; i++) {
    x = 2 * (double) rand() / (RAND_MAX) - 1.0;
    y = 2 * (double) rand() / (RAND_MAX) - 1.0;
    distance_squared = x * x + y * y;
    if (distance_squared <= 1) local_number_in_circle ++;
  }


  //get data from proccesses
  MPI_Reduce(&local_number_in_circle, &total_number_in_circle, 1, MPI_LONG_LONG_INT, MPI_SUM, 0, MPI_COMM_WORLD);


  //print the result
  if (my_rank == 0) {
    printf("pi estimate = %f.\n", 4.0 * (double) total_number_in_circle / total_number_of_toss);
  }

  MPI_Finalize();
  return 0;  
} 