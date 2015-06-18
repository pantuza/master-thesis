/**
 * Implementation of the merge-sort to get the smaller and 
 * higher values inside the array. 
 *
 * Analysis of the merge-sort Loop Invariant and its
 * complexity. 
 * 
 * Loop Invariant (lines 68..81):
 *
 *      Initialization:
 *          
 *          On the first iteration l and m starts with 0. The subarrays
 *          L and R represents the partitions of the Original array:
 *              
 *              A[L,R] => L[l..lsize] and R[m..rsize] 
 *      
 *          As a consequence, L[0] and R[0] are the smallest elements of 
 *          their arrays. The Loop Invariant is valid in prior to the first
 *          iteration.
 *
 *      Maintenance:
 *      
 *          When L[l] <= R[m], A[k] is updated with L[l] value. If A[left..k] 
 *          represents the ordered subarray of A, then this condition keep the
 *          Loop Invariant of the algorithm. The same rule applies to the case
 *          of R[m] > L[l]. On each case, l and m are incremented to satisfy
 *          the next iteration.
 *
 *      Termination:
 *
 *          When k > right then L and R are sorted into A[left..right]. So, the
 *          partition A[left..right] is sorted from the smallest to higher 
 *          element.
 *
 *
 * @author: Gustavo Pantuza
 * @since: 31.03.2013
 *
 */

#include <stdio.h>
#include <string.h>
#include <limits.h>


#define A_size 10


/**
 * Prints elements of the given array starting from 0
 * to size with the prefix string if given
 */
void print_array (int *array, int size, char *prefix)
{
    if(strlen(prefix) > 0) printf("%s:\t", prefix);

    int i = 0;
    printf("[");
    
    for(; i < size - 1; i++) printf(" %d,", array[i]);
    
    printf(" %d ]\n", array[size-1]);
}


/**
 * Merge the partitioned array (larray and rarray) into the original 
 * array (array variable) from left to right
 */
void merge (int *array, int left, int middle, int right)
{
    printf("\n*** MERGING WITH > left: %d, middle: %d, right: %d "
           "***\n", left, middle, right);

    int lsize = middle - left + 1;
    int rsize = right - middle;
    int larray[lsize + 1];
    int rarray[rsize + 1];
    
    int i = 0;
    for(; i < lsize; i++) larray[i] = array[left + i];
    larray[lsize] = INT_MAX;
    print_array(larray, lsize, "L-ARRAY");
    
    int j = 0;
    for(; j < rsize; j++) rarray[j] = array[middle + 1 + j];
    rarray[rsize] = INT_MAX;
    print_array(rarray, rsize, "R-ARRAY");

    int k = left;
    int l = 0;
    int m = 0;

    for(; k <= right; k++)
    {
        if(larray[l] <= rarray[m])
        {
            array[k] = larray[l];
            l++;
        }
        else
        {
            array[k] = rarray[m];
            m++;
        }
        print_array(array, A_size, "MERGE-ARRAY");
    }
}


/**
 * Merge Sort. Make all the recursive calls partioning
 * the array until it can be merged
 */
void sort (int *array, int left, int right)
{
    int middle;

    if(left < right)
    {
        middle = (left + right) / 2;
        sort(array, left, middle);
        sort(array, middle + 1, right);
        merge(array, left, middle, right);
    }
}


void main ()
{
    int A[] = {25, 15, 5, 45, 85, 75, 95, 35, 65, 55};

    printf("\n>>> FIND MAX E MIN WITH MERGE SORT <<<\n");

    print_array(A, A_size, "\nORIGINAL ARRAY");
    sort(A, 0, A_size - 1);
    print_array(A, A_size, "--\n\nSORTED ARRAY");

    printf("\n\tSmaller value: A[0] = %d", A[0]);
    printf("\n\tHigher value: A[%d] = %d", A_size - 1, A[A_size - 1]);
}
