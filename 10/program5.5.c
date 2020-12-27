#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <malloc.h>

int main(int agrc, char* argv[]) {
    int thread_count = strtol(argv[1], NULL, 10);
    int n = strtol(argv[2], NULL, 10);
    int* array = (int *)malloc(n * sizeof(int));
    int temp;
    double start, end;

    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        array[i] = rand() % RAND_MAX;
    }

    start = omp_get_wtime();
    int phase, i;
    #pragma omp parallel num_threads(thread_count) default(none) shared(array, n) private(temp, i, phase)
    for (phase = 0; phase < n; phase++) {
        if (phase % 2 == 0) {
            #pragma omp for
            for (i = 1; i < n; i+=2) {
                if (array[i - 1] > array[i]) {
                    temp = array[i - 1];
                    array[i - 1] = array[i];
                    array[i] = temp;
                }
            }
        } else {
            #pragma omp for
            for (i = 1; i < n; i+=2) {
                if (array[i] > array[i + 1]) {
                    temp = array[i + 1];
                    array[i + 1] = array[i];
                    array[i] = temp;
                }
            }
        }
    }
    end = omp_get_wtime();

    printf("The time is %lf.\n", end - start);

    return 0;
}