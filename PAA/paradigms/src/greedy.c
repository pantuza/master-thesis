#include <limits.h>
#include <string.h>
#include "file.h"
#include "greedy.h"

/*
 * serch_start = search a sequence (size k) from the beginning of word
 *
 * return position of the word (grater than word size means not found)
 * return encountered sequence size (ptrSeq)
 *
 * Parameters:
 * k = minimum sequence size (of the word) to compose the subsequence
 * word = search word
 * sw = size of the search word
 * subseq = word with subsequence at the start
 * ss = size of word with subsequence at the start
 * ptrSeq = pointer to the encountered sequence size
 *
 */
int serch_start(int k, const char* word, int sw,
        const char* subseq, int ss, int* ptrSeq)
{
    int seq = 0;
    // from left to the right, does the search...
    for (int j = 0; j < sw; j++)
        if (*subseq == word[j]) {
            // how far is the matching sequence?
            for (seq = 1;
                    word[j+seq] && subseq[seq] &&
                    subseq[seq] == word[j+seq];
                    seq++);
            // is sequence acceptable?
            if (seq >= k)
            {
                // found a sequence
                *ptrSeq = seq;
                return j+seq;
            }
        };
    // not found
    *ptrSeq = 0;
    return sw+1;
}


/*
 * serch_end = search a sequence (size k) from the ending of word
 *
 * return position of the word (less than zero means not found)
 * return encountered sequence size (ptrSeq)
 *
 * Parameters:
 * k = minimum sequence size (of the word) to compose the subsequence
 * word = search word
 * sw = size of the search word
 * subseq = word with subsequence at the end
 * ss = size of word with subsequence at the end
 * ptrSeq = pointer to the encountered sequence size
 *
 */
int serch_end(int k, const char* word, int sw,
        const char* subseq, int ss, int* ptrSeq)
{
    int seq = 0;
    ss--; // if ss is size, subseq[ss] is zero... "ss - 1" is the last index!
    // from right to the left of word, does the search...
    for (int j = (sw - 1); j >= 0; j--)
        // match found
        if (subseq[ss] == word[j]) {
            // how far is the matching sequence?
            for (seq = 1;
                    (j-seq) >= 0 && (ss-seq) >= 0 &&
                    subseq[ss-seq] == word[j-seq];
                    seq++);
            // is sequence acceptable?
            if (seq >= k)
            {
                // found a sequence
                *ptrSeq = seq;
                return j-seq+1;
            }
        };
    // not found
    *ptrSeq = 0;
    return -1;
}

/*
 * greed_choice = make a greedy choice according to current words
 *
 * return a size of the sequence that is a partial solution
 *        (added to solution of lcs)
 * return word0, sw0, word1 and sw1 without the adopted sequence (pointers)
 *
 * Parameters:
 * k = minimum sequence size (of the word) to compose the subsequence
 * word0 = pointer to the first word
 * sw0 = pointer to the size of the first word
 * word1 = pointer to the second word
 * sw1 = pointer to the size of the second word
 *
 */
int greedy_choice(int k, char **word0, int *sw0, char **word1, int *sw1)
{
    int seqS, seqE;
    int posS, posE;

    int deltaS, deltaE;
    // search the initial sequence (with size >= k) of word0 in word2
    do
    {
        posS = serch_start(k, *word1, *sw1, *word0, *sw0, &seqS);
        // found?
        if (posS > *sw1)
        {
            // remove the first character from word0
            (*sw0) -= 1;
            (*word0) += 1;
            if (*sw0 > 0)
                // if the word0 is not empty, searches again!
                deltaS = -1;
            else
                // if word0 is done... the choice is done too...
                return 0;
        }
        else
        {   // if initial sequence is found, calculates the delta
            deltaS = posS - seqS;
        }
    } while (deltaS < 0);

    // search the terminal sequence (with size >= k) of word0 in word2
    do
    {
        posE = serch_end(k, *word1, *sw1, *word0, *sw0, &seqE);
        // found?
        if (posE < 0)
        {
            // remove the last character from word0
            (*sw0) -= 1;
            (*word0)[(*sw0)] = 0;
            if (*sw0 > 0)
                // if the word0 is not empty, searches again!
                deltaE = -1;
            else
                // if word0 is done... the choice is done too...
                return 0;
        }
        else
        {   // if terminal sequence is found, calculates the delta
            deltaE = *sw1 - posE - seqE;
        }
    } while (deltaE < 0);

    // which delta is smaller?
    if (deltaS < deltaE)
    {
        // if deltaS, remove sequence of the start
        (*sw0) -= seqS;
        (*word0) += seqS;
        (*sw1) -= posS;
        (*word1) += posS;
        return seqS;
    }
    else
    {
        // if deltaE, remove sequence of the ending
        (*sw0) -= seqE;
        (*word0)[*sw0] = 0;
        (*sw1) = posE;
        (*word1)[*sw1] = 0;
        return seqE;
    }
}

/*
 * greed = calculate LCS with greedy approach
 *
 * Parameters:
 * k = minimum sequence size (of the word) to compose the subsequence
 * word0 = first word
 * sw0 = size of the first word
 * word1 = second word
 * sw1 = size of the second word
 *
 */
int greedy(int k, const char *word0, int sw0, const char *word1, int sw1)
{
    // make a copy os words
    char *w0 = malloc(sw0+1);
    char *w1 = malloc(sw1+1);
    char *original0 = w0;
    char *original1 = w1;
    strncpy(w0, word0, sw0+1);
    strncpy(w1, word1, sw1+1);
    int s0 = sw0, s1 = sw1;
    int lcs = 0;
    
    // while has a partial-solution (word0 is not done)
    do {
        // accumulate all partial solutions from greedy choices
        lcs += greedy_choice(k, &w0, &s0, &w1, &s1);
    } while(*w0);
    
    // relase memory
    free(original0);
    free(original1);
    
    // return the greed solution
    return lcs;
}


