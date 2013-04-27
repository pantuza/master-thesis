/**
 * Analysis of Selection Sort implementation about 
 * its Loop Invariants, number of comparisons and 
 * exchanges.
 */ 
#include <stdio.h>


#define n_items 10


void print_array (int *array)
{
    int i = 0;

    printf("\n[ ");
    
    for(; i<n_items-2; i++) 
        printf("%d, ", array[i]);

    printf("%d ]\n", array[n_items-1]);
}

void selection_sort (int *array)
{
    int min,
        tmpval, 
        i = 0, 
        j = 0;

    /**
     * Loop Invariant
     *
     * Initialization:
     *      i=0 and i <= n
     * Maintenance:
     *      i++
     * Finalization:
     *      i>n
     */
    for(; i<=n_items-1; i++) { // n comparison
        min = i;
        /**
         * Loop Invariant
         *
         * Initialization:
         *      j=i+1 e j <= n
         * Maintenance:
         *      j++
         * Finalization:
         *      j>n
         */
        for(j=i+1; j<=n_items-1; j++)

            if(array[j] < array[min]) 
                min = j;

        // exchange
        if(min != i)
        {
            tmpval = array[i];
            array[i] = array[min];
            array[min] = tmpval;
        }
    }
}

void main (int argc, char *argv[])
{
    int A[] = {5,4,10,38,22,12,50,67,22,1};

    print_array(A);
    selection_sort(A);
    print_array(A);
}
