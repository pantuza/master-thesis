#include <stdio.h>

#define A_size 10

void printA (int *array)
{
    int i = 0;
    printf("[");
    for(; i<A_size-1; i++)
        printf(" %d,", array[i]);
    printf(" %d ]\n", array[A_size-1]);
}

void main ()
{
    int A[] = {20, 40, 10, 30, 0, 90, 70, 60, 50, 80};
    int i, min, max = 0;

    printA(A);

    /**
     * Loop Invariant: 
     *
     * Initialization: 
     *     . The first iteration of the loop we have the max and min values
     * pointed to the first element of A. If the i element in A is greater 
     * then max element then max become i. Otherwise if the i element in A
     * is less the min element then min become i. The iteration is true 
     * prior to the first iteration. 
     *
     * Maintenance:
     *     . For each iteration the loop invariant is maintained. For i 
     * from 0 to the length of A ([i=0 .. i=9]), if there is a number 
     * less or greater then max or min, it will be assigned to max or min
     * respectively. 
     *
     * Termination:
     *     . The iteration finishes when i becomes A_size that represents
     * the last index inside A. Then, if the entire array A was traversed, 
     * then min and max has the lowest and highest values of the array A.
     *
     */
    for(; i<A_size; i++)
    {
        if(A[i] > A[max])
            max = i;
        else if(A[i] < A[min])
            min = i;
    }

    printf("Min: %d, Max: %d\n", A[min], A[max]);
}
