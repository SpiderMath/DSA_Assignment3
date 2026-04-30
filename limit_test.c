#include <stdio.h>

/**
 * Main idea:
 * Following the Assignment Question, we must first test the upper limit of n such that n! cannot be computed directly.
 * To do this, we use the largest standard integer data type in C (unsigned long long) to see exactly when the math overflows.
 * Since 'unsigned long long' is 64 bits, it can hold a number up to 1.8 x 10^19.
 * We will continuously calculate factorials until we detect an overflow. We will detect this by doing the inverse operation (division) 
 * and checking if it matches the previous state to prove the data type has failed us.
 */

int main() {
    unsigned long long factorial = 1; 
    unsigned long long previous = 1;

    int n = 1;

    printf("Testing factorial limits in standard C...\n\n");

    while (1) {
        // Standard factorial stuff
        previous = factorial;
        factorial = factorial * n; 

        // Main task: checking for overflow
        // if the number gets too big for the 64-bit data type, it overflows and gives a garbage result, and we test this by dividing our new factorial by n, it should equal our previous number.
        if (factorial / n != previous) {
            printf("\nOVERFLOW DETECTED\n");
            printf("The memory limit was hit when trying to calculate %d!\n", n);
            printf("The highest successful calculation was for %d! \n", n - 1);
            break; 
        }

        // no overflow case
        printf("%d! = %llu\n", n, factorial);
        n++;
    }

    return 0;
}