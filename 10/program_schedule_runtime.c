#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    int thread_count = strtol(argv[1], NULL, 10);
    int n = strtol(argv[2], NULL, 10);

#pragma omp parallel for num_threads(thread_count) schedule(runtime)
    for (int i = 0; i < n; i ++) {
        printf("i=%d, thread_id=%d\n", i, omp_get_thread_num());
    }

    return 0;
}