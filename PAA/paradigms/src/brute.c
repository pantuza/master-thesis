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
 * word = word to compare
 * subseq = subsequence to verify
 *
 * Analysis of time growth rates (time complexity):
 * T(n) = O(n)
 *
 * Analysis of space growth rates (space complexity):
 * T(n) = O(1)
 *
 */
int compare(int k, const char* word, const char* subseq)
{
    int j = 0, seq = 0;
    for (; *word; word++)
        // subsequence character matching the word?
        if (subseq[j] == *word) {
            // increase subsequence position
            j++;
            seq++;
            // subsequence finished?
            if (!subseq[j])
            {
                // is sequence large enough to complete the subsequence?
                if (seq >= k) 
                {
                    return j;
                }
                else  // otherwise, try match again in the rest of word
                {
                    // backtrack subsequence an start a new sequence
                    j -= seq;
                    seq = 0;
                }
            }
        }
        else
        {
            // not matching: it is always a new sequence!
            // if isn't a new sequence
            if (seq) {
                // backtrack if wasn't completed the last subsequence
                if (seq < k)
                    j -= seq;
                // start a new sequence
                seq = 0;
            }

        };
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
int subsequence(int k, const char* word, const char* other, char* subseq,
        int n, int seq, int lcs)
{
    // is recursion "bottoms out"?
    // (base case, smallest problem, stopping condition or
    // leaf nodes of recursion tree)
    // in this case, the question is: the word was completed?
    if (!*word)
    {
        // if subsequence size is greater than current LCS (see comments above)
        if (n > lcs) {
            // marks the end of sequence
            subseq[n] = 0;
            // if is subsequence of the other word
            if (compare(k, other, subseq))
                // then, the size of subsequence is as a LCS candidate
                return n;
        // otherwise, return current LCS or
        // return 0 (that's always a candidate to LCS)
        }
        return lcs;
    }
    // increase the recursion tree (to both sides)
    int lcs0, lcs1;

    // 1) with character of the word (to the left on recursion tree)
    // puts current character at the end of subsequence
    subseq[n] = *word;
    // makes a recursive solution of the left branch
    lcs0 = subsequence(k, word + 1, other, subseq, n + 1, seq + 1, lcs);

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
        // removes current character from subsequence
        subseq[n] = 0;
        // makes a recursive solution of the right branch
        lcs1 = subsequence(k, word + 1, other, subseq, n, 0, lcs);
    }

    // return the longest common sequence of the branches
    if (lcs0 > lcs1)
        return lcs0;
    return lcs1;
}

/*
 * solution method
 */
int brute_force(int k, const char *word0, int sw0, const char *word1, int sw1)
{
    //
    char subseq[WORD_SIZE + 1];
    return subsequence(k, word0, word1, subseq, 0, 0, 0);
}

/*
 * main
 */
int main(int argc, char** argv)
{
    sweep_input(stdin, brute_force);
    return 0;
}

