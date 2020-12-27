#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

long long int monte_carlo();

int main(int argc, char *argv[])
{
    long long int total_number_of_toss = strtol(argv[1], NULL, 10);
    long long int total_number_in_circle = 0;
    int thread_count = strtol(argv[2], NULL, 10);

    srand(time(NULL));

#pragma omp parallel for num_threads(thread_count) reduction(+: total_number_in_circle)
    for (int toss = 0; toss < total_number_of_toss; toss++)
    {
        total_number_in_circle += monte_carlo();
    }

    printf("pi estimate = %f.\n", 4.0 * (double)total_number_in_circle / total_number_of_toss);
    return 0;
}

long long int monte_carlo()
{
    double x, y, distance_squared;
    x = 2 * (double)rand() / (RAND_MAX)-1.0;
    y = 2 * (double)rand() / (RAND_MAX)-1.0;
    distance_squared = x * x + y * y;
    if (distance_squared <= 1)
        return 1;
    else
        return 0;
}