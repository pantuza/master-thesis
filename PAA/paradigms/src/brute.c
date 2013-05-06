#include "file.h"
#include "brute.h"
#include <math.h>


char** alloc_subseqs(int size)
{
    int i = 0;
    char **seqs = (char**) malloc(size * sizeof(char*));

    for(;i < size;i++)
        subseqs0[i] = (char**) malloc(size * sizeof(char*));

    return seqs;
}

int brute_force(int segment_size, Word *word0, Word *word1)
{
    int i = 0;
    int j = 0;

    char **subseqs0 = alloc_subseqs(word0->length);

    while(i < word0->length)
        while(j < word0->length)
            int k = i;
            for(; k <= j; k++)
    }
}

void sweep_input(FILE *file)
{
    int LCS;
    int segment_size = get_seq_size(file);

    while(segment_size != 0)
    {
        Word word0 = word_handler(file);
        Word word1 = word_handler(file);

        LCS = brute_force(segment_size, &word0, &word1);

        segment_size = get_seq_size(file);
        
        printf("k: %d\nword0: %sword1: %sLCS: %d\n", 
                segment_size, word0.letters, word1.letters, LCS);
    }
}


int main(int argc, char argv[])
{
    FILE *file = open_file("inputs/00_small_input");
    sweep_input(file);
    close_file(file);
    return 0;
}
