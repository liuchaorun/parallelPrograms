#include <stdio.h>
#include <malloc.h>

int main() {
    double *vector, *prefixSums;
    int  n;

    printf("Please input the order of the vectors:\n");
    scanf("%d", &n);
    vector = (double *)malloc(n * sizeof(double));
    prefixSums = (double *)malloc(n * sizeof(double));
    printf("Please input the vector:\n");
    for (int i = 0; i < n; i++) {
        scanf("%lf", &vector[i]);
    }

    prefixSums[0] = vector[0];
    for (int i = 1; i < n; i++) {
        prefixSums[i] = prefixSums[i - 1] + vector[i]; 
    }

    printf("The result of prefix sums is: ");
    for (int i = 0; i < 0; i++) {
        printf("%lf ", vector[i]);
    }
    printf("\n");

    return 0;
}