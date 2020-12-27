#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char* argv[]) {
    int thread_count = strtol(argv[1], NULL, 10);

#pragma omp parallel for num_thread(thread_count) schedule(static, 1)
    for (int i = 0; i < 100; i ++) {

    }

    return 0;
}