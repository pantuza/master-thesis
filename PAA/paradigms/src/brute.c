#include <math.h>
#include "file.h"
#include "brute.h"
#include "strtype.h"

#define NO_SEQUENCE 0
#define IN_SEQUENCE 1

/*
 * compare = verify if is subsequence of the word.
 *           return true (size of subsequence) or false (0)
 * Parameters:
 * k = minimum sequence size (of the word) to compose the subsequence
 * word = word of the subsequence
 * other = word to compare
 * subseq = subsequence to verify
 * n = size of subsequence
 *
 * Analysis of time growth rates (time complexity):
 * T(n) = O(n)
 *
 * Analysis of space growth rates (space complexity):
 * T(n) = O(1)
 *
 */
int compare(int k, const char *word, const char* other, int* subseq, int n)
{
    int i, j = 0, seq = 0;
    char c0, c1;

    while(*other)
        // subsequence character matching the word?
        if (word[subseq[j]] == *other) {
            // verify if sequences match
            seq = 1;
            i = j + seq;
            // while search in subsequence
            while (i < n)
            {
                if (seq >= 2*k)
                {
                    j += k;
                    other += k;
                    seq -= k;
                }
                c0 = word[subseq[i]];
                c1 = other[seq];
                // if other word is over without complete actual sequence...
                if (!c1)
                {
                    // the comparison is ended!
                    return 0;
                }
                // if word of subsequence ended...???
                // panic!!!! this just can't happening or subsequence is crazy.
                // if keep (john) walking in sequence...
                if (c0 == c1)
                    // and the subsequence remains in sequence?
                    if ((subseq[i] - subseq[i-1]) == 1)
                    {
                        // just remember this in seq
                        seq++;
                        i = j + seq;
                    }
                    else
                    {
                        // sequence less than k?
                        if (seq < k)
                        {
                            i = j;  // backtrack j
                            seq = 1;
                        }
                        break;
                    }
                // but, the sequence is broken
                else
                    if (seq >= k) {
                        // and the subsequence remains in sequence?
                        if ((subseq[i] - subseq[i-1]) == 1)
                        {
                            int remainder = 1;
                            int l = i + remainder;
                            while ((l < n) &&
                                    (remainder < k) &&
                                    ((subseq[l] - subseq[l-1]) == 1))
                            {
                                remainder++;
                                l = i + remainder;
                            }
                            if (remainder < k)
                            {
                                i = j;  // backtrack j
                                seq = 1;
                            }
                            break;
                        }
                        else
                        {
                            break;
                        }
                    }
                    else
                    {
                        i = j;  // backtrack j
                        seq = 1;
                        break;
                    }
            }
            j = i;
            other += seq;
            // subsequence finished?
            if (j >= n)
            {
                // is sequence large enough to complete the subsequence?
                // irrelevant!!!! it always must to be true in here.
                //if (seq >= k)
                //{
                return n;
                //}
            }
        }
        else
        {
            other++;
        }
    // if it reaches here, the subsequence doesn't match the word
    return 0;
}

/*
 * subsequence = brute force recursion tree to discover LCS
 * Return: LCS
 * Parameters:
 * k = minimum sequence size (of the word) to compose the subsequence
 * word = pointer to first character of word
 *        which will be generated all subsequences combinations
 * other = word to compare with the subsequence
 * subseq = subsequence available so far
 * n = last index (position) of subsequence at the moment
 * seq = current sequence size (condition: seq >= k)
 * lcs = longest common subsequence so far
 *       since the longest subsequences  will be compared first,
 *       this optional optimization can avoiding dispensable comparisons
 *
 * Analysis:
 * let n = size of word
 * let m = size of other word
 *
 * Analysis of time growth rates (time complexity in worst case):
 * T(1) = O(m)                         (from compare function)
 * T(n) = 2T(n - 1) + O(1)
 *      = 2(2(2...(T(1) + O(1))... + O(1)) + O(1)) + O(1)
 *      = 2^(n - 1)(O(m) + O(1)) + O(1)
 *      < 2^n(O(m))
 *      = O((2^n)*m)
 * Note: Equivalent to compare all leaf nodes (all subsequences) with word.
 *
 * Analysis of space growth rates (space complexity):
 * T(1) = O(1)                         (from compare function)
 * T(n) = T(n - 1) + O(1)
 *      = (((T(1) + O(1))...) + O(1)) + O(1)
 *      = (((O(1) + O(1))...) + O(1)) + O(1)
 *      = n*(O(1))
 *      = O(n)
 * Note: Equivalent to one branch (one subsequence) of recursion tree.
 *
 */
int subsequence(int k, const char* word, const char* other, int so, int* subseq,
        int deep, int n, int seq, int *lcs)
{
    // is recursion "bottoms out"?
    // (base case, smallest problem, stopping condition or
    // leaf nodes of recursion tree)
    // in this case, the question is: the word was completed?
    if (!word[deep])
    {
        // if subsequence size is greater than current LCS (see comments above)
        // and subsequence is empty or greater or equal k (valid subsequence)
        if ((n > *lcs) && (n <= so) && (!seq || seq >=k)) {
            // if is subsequence of the other word
            if (compare(k, word, other, subseq, n))
            {
                //
                if (*lcs < n)
                    *lcs = n;
                // then, the size of subsequence is as a LCS candidate
                return n;
            }
        // otherwise, return current LCS or
        // return 0 (that's always a candidate to LCS)
        }
        return *lcs;
    }
    // increase the recursion tree (to both sides)
    int lcs0, lcs1;

    // 1) with character of the word (to the left on recursion tree)
    // puts current character at the end of subsequence
    subseq[n] = deep;
    // makes a recursive solution of the left branch
    lcs0 = subsequence(k, word, other, so, subseq, deep + 1, n + 1, seq + 1, lcs);

    // 2) without character of the word  (to the right on recursion tree)

    // when algorithm enters a right node, (exactly the next move)
    // if already has a non empty sequence in branch, the the current sequence,
    // or even the entirely combination, is finished. If current sequence size
    // is 0<seq<k (non empty and less than enough) the subsequence is invalid,
    // as all combination from it. In this case, the branch can be skipped,
    // including it's sub-tree.
    if (seq && (seq < k))
        lcs1 = 0;
    // otherwise, go through the right branch:
    else
    {
        // removes last character from subsequence
        subseq[n] = 0;
        // makes a recursive solution of the right branch
        lcs1 = subsequence(k, word, other, so, subseq, deep + 1, n, 0, lcs);
    }

    // return the longest common sequence of the branches
    if (lcs0 > lcs1)
        return lcs0;
    return lcs1;
}

/*
 * brute_force = calculate LCS by verification in all possibles solutions
 *
 * Parameters:
 * k = minimum sequence size (of the word) to compose the subsequence
 * word0 = first word
 * sw0 = size of the first word
 * word1 = second word
 * sw1 = size of the second word
 *
 */
int brute_force(int k, const char *word0, int sw0, const char *word1, int sw1)
{
    //
    int subseq[WORD_SIZE + 1];
    int lcs = 0;
    return subsequence(k, word1, word0, sw0, subseq, 0, 0, 0, &lcs);
}


