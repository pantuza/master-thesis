#include "file.h"
#include "brute.h"
#include <math.h>

#define NO_SEQUENCE 0
#define IN_SEQUENCE 1

/*
 * verify = verify if subsequence contains only sequences larger or equals k
            return true (not 0) or false (0)
 * Parameters:
 * word = word to compare
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
int validate(int k, int subseq[], int n)
{
    int seq = 1;
    // for every subsequence element
    for (int i = 0; i < n; i++)
        // if it's a sequence
        if (subseq[i] == (subseq[i+1] - 1))
            // increase sequence size control
            seq++;
        else
            // the last sequence is large enough?
            if (seq < k)
                // NO... verification fails! It's NOT a valid subsequence
                return 0;
            else
                // YES! restart the sequence and continue
                seq = 1;
    return 1;
}
/*
 * compare = verify if is subsequence of the word.
 *           return true (size of subsequence) or false (0)
 * Parameters:
 * word = word to compare
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
int compare(int k, char* other, char* word, int subseq[], int n)
{
    int j = 0, seq = 0;
    for (int i = 0; other[i]; i++)
        // subsequence character matching the word?
        if (word[subseq[j]] == other[i])
        {
            // increase subsequence position
            j++;
            seq++;
            // subsequence finished?
            if (j >= n)
            {
                // is sequence large enough to complete the subsequence?
                if (seq >= k) {
                    return n;
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
            if (seq)
            {
                // backtrack if wasn't completed the last subsequence
                if (seq < k)
                    j -= seq;
                // start a new sequence
                seq = 0;
            }

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
 * depth = recurrence tree height (word vector index)
 * in_sequence = Is subsequence in sequence of the word?
 * lcs = longest common subsequence so far
 *       since the longest subsequences  will be compared first,
 *       this optional optimization can avoiding dispensable comparisons
 *
 * Analysis of time growth rates (time complexity):
 * T(1) = O(n) + O(n) = O(n) (from validate and compare functions)
 * T(n) = 2T(n - 1) + O(1)
 *      = 2(2(2...(T(1) + O(1))... + O(1)) + O(1)) + O(1)
 *      = 2^(n - 1)(O(n) + O(1)) + O(1)
 *      < 2^n(O(n))
 *      = O((2^n)*n)
 * Note: Equivalent to compare all leaf nodes (all subsequences) with word.
 *
 * Analysis of space growth rates (space complexity):
 * T(1) = O(1) + O(1) = O(1)  (from validate and compare functions)
 * T(n) = T(n - 1) + O(1)
 *      = (((T(1) + O(1))...) + O(1)) + O(1)
 *      = (((O(1) + O(1))...) + O(1)) + O(1)
 *      = n*(O(1))
 *      = O(n)
 * Note: Equivalent to one branch (one subsequence) of recursion tree.
 *
 */
int subsequence(int k, char* word, char* other,
        int subseq[], int n, int depth, int lcs)
{
    // is recursion "bottoms out"?
    // (base case, smallest problem, stopping condition or
    // leaf nodes of recursion tree)
    // in this case, the question is: the word was completed?
    if (!word[depth])
    {
        // if subsequence size is greater than current LCS (see comments above)
        if (n > lcs)
            // if subsequence contains only sequences sizes greater or equals k
            if (validate(k, subseq, n))
                // if is subsequence of the other word
                if (compare(k, other, word, subseq, n))
                    // then, the size of subsequence is as a LCS candidate
                    return n;
        // otherwise, return current LCS or
        // return 0 (that's always a candidate to LCS)
        return lcs;
    }
    // increase the recursion tree (to both sides)
    int lcs0, lcs1;

    // 1) with character of the word (to the left on recursion tree)
    // puts current character in subsequence
    subseq[n] = depth;
    // makes a recursive solution of the left branch
    lcs0 = subsequence(k, word, other, subseq, n + 1, depth + 1, lcs);

    // 2) without character of the word  (to the right on recursion tree)
    // removes current character from subsequence 
    subseq[n] = 0;
    // makes a recursive solution of the right branch
    lcs1 = subsequence(k, word, other, subseq, n, depth + 1, lcs);

    // return the longest common sequence of the branches
    if (lcs0 > lcs1)
        return lcs0;
    return lcs1;
}

/*
 * solution method
 */
int brute_force(int segment_size, char *word0, char *word1)
{
    //
    int subseq[WORD_SIZE];
    return subsequence(segment_size, word0, word1, subseq, 0, 0, 0);
}

/*
 * main
 */
int main(int argc, char** argv)
{
    //FILE *file = open_file("00_small_input");
    sweep_input(stdin, brute_force);
    //close_file(file);
    return 0;
}
