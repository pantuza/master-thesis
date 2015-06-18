#include <math.h>
#include <string.h>
#include "file.h"
#include "brute.h"
#include "strtype.h"

#define NO_SEQUENCE 0
#define IN_SEQUENCE 1

/*
 * in_common = number of common character in position and valid sequence
 * Return: integer with count of the common subsequence
 * Parameters:
 * k = minimum sequence size (of the word) to compose the subsequence
 * word = pointer to first character of word
 *        which will be generated all subsequences combinations
 * subseq = subsequence of the word
 * n = size of the word subsequence
 * other = word to compare with
 * osubseq = subsequence of other word
 * so = size of other word
 * IMPORTANT PRECONDITION: n == so
 *
 * Analysis of time growth rates (time complexity in worst case):
 * T(n) = O(n)
 *
 * Analysis of space growth rates (space complexity):
 * T(n) = O(1)
 *
 */
int in_common(int k, const char *word, int *subseq, int n,
        const char *other, int *osubseq, int so)
{
    int i;
    int s = 0;
    int c = 0;
    // sweep the subsequence
    for (i = 0; i < n; i++)
    {
        // if characters in position (i) are equals
        if (word[subseq[i]] == other[osubseq[i]])
        {
            // increase a sequence control
            s++;
            // if sequence is broken
            if (subseq[i+1] - subseq[i] != osubseq[i+1] - osubseq[i])
            {
                // if has a valid sequence
                if (s >= k)
                    // count the sequence size
                    c += s;
                // start a new sequence
                s = 0;
            }
        }
        // if characters in position (i) are diferents
        else
            // ensure a new sequence
            s = 0;
    }
    // if ends with a valid sequence
    if (s >= k)
        // count it too
        c += s;
    return c;
}

/*
 * subsubsequence = brute force recursion tree to discover LCS
 * Return: LCS
 * Parameters:
 * k = minimum sequence size (of the word) to compose the subsequence
 * word = pointer to first character of word
 *        which will be generated all subsequences combinations
 * other = word to compare with the subsequence
 * osubseq = subsequence of other word
 * so = size of other word
 * subseq = subsequence available so far
 * deep = recursion tree deep (word control)
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
 * T(1) = O(m)                         (from is_common  function)
 * T(n) = 2T(n - 1) + O(1)
 *      = 2(2(2...(T(m) + O(1))... + O(1)) + O(1)) + O(1)
 *      = 2^(n - 1)(O(m) + O(1)) + O(1)
 *      < 2^n(O(m))
 *      = O((2^n)*m)
 *      = O(n(2^n))     (assuming m = n)
 * Note: Equivalent to compare all leaf nodes (all subsequences) with word.
 *
 * Analysis of space growth rates (space complexity):
 * T(1) = O(1)                         (from is_common function)
 * T(n) = T(n - 1) + O(1)
 *      = (((T(1) + O(1))...) + O(1)) + O(1)
 *      = (((O(1) + O(1))...) + O(1)) + O(1)
 *      = n*(O(1))
 *      = O(n)
 * Note: Equivalent to one branch (one subsequence) of recursion tree.
 *
 */
int subsubsequence(int k, const char* word,
        const char* other, int *osubseq, int so,
        int* subseq, int deep, int n, int seq, int *lcs)
{
    // see comments below in subsequence function!
    if (!word[deep])
    {
        if ((n > *lcs) && (n == so) && (!seq || seq >=k))
        {
            subseq[n] = 0;
            return in_common(k, word, subseq, n, other, osubseq, so);;
        }
        return 0;
    }
    int lcs0, lcs1;
    subseq[n] = deep;
    lcs0 = subsubsequence(k, word, other, osubseq, so,
            subseq, deep + 1, n + 1, seq + 1, lcs);
    if (seq && (seq < k))
        lcs1 = 0;
    else
    {
        subseq[n] = 0;
        lcs1 = subsubsequence(k, word, other, osubseq, so,
                subseq, deep + 1, n, 0, lcs);
    }
    if (lcs0 > lcs1)
        return lcs0;
    return lcs1;
}

/*
 * subsequence = brute force recursion tree to discover LCS
 * Return: LCS
 * Parameters:
 * k = minimum sequence size (of the word) to compose the subsequence
 * word = pointer to first character of word
 *        which will be generated all subsequences combinations
 * other = word to compare with the subsequence
 * so = size of other word
 * subseq = subsequence available so far
 * deep = recursion tree deep (word control)
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
 * T(1) = O(m(2^n))                         (from subsubsequence function)
 * T(n) = 2T(n - 1) + O(1)
 *      = 2(2(2...(T(1) + O(1))... + O(1)) + O(1)) + O(1)
 *      = 2^(n - 1)(O(m(2^n)) + O(1)) + O(1)
 *      < 2^n(O(m(2^n)))
 *      = O(m(2^(2n)))
 *      = O(n(2^(2n)))  (assuming m = n)
 * Note: Equivalent to compare all leaf nodes (all subsequences) with word.
 *
 * Analysis of space growth rates (space complexity):
 * T(1) = O(n)                         (from subsubsequence function)
 * T(n) = T(n - 1) + O(1)
 *      = (((T(1) + O(1))...) + O(1)) + O(1)
 *      = (((O(1) + O(1))...) + O(1)) + O(1)
 *      = n*(O(n))
 *      = O(n^2)
 * Note: Equivalent to one branch (one subsequence) of recursion tree.
 *
 */
int subsequence(int k, const char* word,
        const char* other, int so,
        int* subseq, int deep, int n, int seq, int *lcs)
{
    // is recursion "bottoms out"?
    // (base case, smallest problem, stopping condition or
    // leaf nodes of recursion tree)
    // in this case, the question is: the word was completed?
    if (!word[deep])
    {
        // if:
        // subsequence size is greater than current LCS (see comments above)
        // and subsequence is less or equal other word (lcs candidate)
        // and subsequence is empty or greater or equal k (valid subsequence)
        if ((n > *lcs) && (n <= so) && (!seq || seq >=k)) {
            // ensure that the subsequence has a termination mark
            subseq[n] = 0;
            // other subsequence to compare
            int subsubseq[WORD_SIZE + 1];
            // if is subsequence of the other word
            if (subsubsequence(k, other,
                    word, subseq, n,
                    subsubseq, 0, 0, 0, lcs) == n)
            {
                // set lcs in advance
                if (*lcs < n)
                    *lcs = n;
                // then, the size of subsequence is as a LCS candidate
                return n;
            }
        // otherwise, return current LCS or
        // return 0 (that's always a candidate to LCS)
        }
        return 0;
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
