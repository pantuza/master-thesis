#include <math.h>
#include "file.h"
#include "dynamic.h"

/*
 * dynamic = calculate LCS with dynamic programming
 *
 * Parameters:
 * k = minimum sequence size (of the word) to compose the subsequence
 * word0 = first word
 * sw0 = size of the first word
 * word1 = second word
 * sw1 = size of the second word
 *
  * Analysis:
 * let n = size of the first word
 * let m = size of the second word
 *
 * Analysis of time growth rates (time complexity in worst case):
 * T(n) = (n+1)O(1) + (m+1)O(1) + ((n+1)*(m+1))O(1) + ((n+1)*(m+1))O(1) + O(1)
 *      = (n+1)O(1) + (m+1)O(1) + 2((n+1)*(m+1))O(1) + O(1)
 *      = O(n) + O(m) + O(2nm) + O(1)
 *      = O(2nm)
 *
 * Analysis of space growth rates (space complexity):
 * T(n) = (n+1)(m+1)O(1) + (n+1)(m+1)O(1) + O(1)
 *      = 2(n+1)(m+1)O(1) + O(1)
 *      = O(2nm) + O(1)
 *      = O(2nm)
 *
 */
int dynamic(int k, const char *word0, int sw0, const char *word1, int sw1)
{
    int i, j;
    int t[sw0+1][sw1+1];
    int c[sw0+1][sw1+1];

    // initialize the matrix of sum (character and sequences in common)
    for (i = 0; i <= sw0; i++)
        t[i][0] = c[i][0] = 0;
    for (j = 0; j <= sw1; j++)
        t[0][j] = c[0][j] = 0;

    const char *p0;
    const char *p1;

    // mount the matrix "s" with counts of matches between words
    p0 = word0;
    for (i = 1; *p0; i++, p0++) {
        p1 = word1;
        for (j = 1; *p1; j++, p1++) {
            if (*p0 == *p1)
                c[i][j] = c[i-1][j-1] + 1;
            else
                c[i][j] = 0;
        }
    }

    // mount the matrix "t" with LCS considering matches in sequence >= k
    p0 = word0;
    for (i = 1; *p0; i++, p0++) {
        p1 = word1;
        for (j = 1; *p1; j++, p1++) {
            t[i][j] = MAX(t[i-1][j], t[i][j-1]);
            for(int seq=k; seq<=c[i][j]; seq++)
                t[i][j] = MAX(t[i][j], t[i-seq][j-seq]+seq);
        }
    }

    return t[sw0][sw1];
}


