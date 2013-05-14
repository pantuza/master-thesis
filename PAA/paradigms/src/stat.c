#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>

#include "chronos.h"
#include "strtype.h"
#include "file.h"
#include "dynamic.h"
#include "brute.h"
#include "greedy.h"

/*
 * execute the method to calculate LCS for all input cases
 */
void sweep_input_test(FILE *file, method execute, 
                      char *mode, FILE* stat, FILE *result)
{
    int lcs;
    // the string must contain space for termination mark ('\0')
    char word0[WORD_SIZE + 1], word1[WORD_SIZE + 1];
    int size0, size1;

    // read the first k (zero implies end of execution)
    int segment_size = get_segment_size(file);

    // while exist input case to execute (test case)
    while(segment_size != 0)
    {
        // reads both words
        size0 = get_word(file, word0);
        size1 = get_word(file, word1);

        // is really there words?
        if (size0 && size1) {
            start_timer();
            // sort the arguments (words)
            if (strlen(word0) <= strlen(word1))
                lcs = execute(segment_size, word0, size0, word1, size1);
            else
                lcs = execute(segment_size, word1, size1, word0, size0);
            stop_timer();
            write_stat(stat, "tp1", mode, size0, size1, segment_size, 0);
            // outputs the calculated size of longest common subsequence
            fprintf(result, "%d\n", lcs);
        }
        else
        {
            // returns zero, that always is a valid size of LCS,
            // even in this system error condition (empty line or word)
            fprintf(result, "0");
        }

        // read another instance of k (zero implies end of execution)
        segment_size = get_segment_size(file);
    }
}

void do_test(char *file, char *mode, method execute) 
{
    char filename[1024];
    char stat_filename[1024];
    char result_filename[1024];

    sprintf(filename, "inputs/%s", file);
    sprintf(stat_filename, "outputs/%s-%s-stat.txt", file, mode);
    sprintf(result_filename, "outputs/%s-%s-result.txt", file, mode);

    printf("[%s] Input: %s \t Outputs: %s, %s\n", mode, filename, 
                     stat_filename, result_filename);
    fflush(stdout);
    FILE *f, *s, *r;
    r = fopen(result_filename, "w");
    s = open_stat_file(stat_filename);
    f = open_file(filename);
    sweep_input_test(f, execute, mode, s, r);
    close_file(f);
    fclose(r);
    fclose(s);
}

int main(int argc, char** argv)
{
    if (argc > 1)
    {
        char* filename = argv[1];
        do_test(filename, "brute_force", brute_force);
        do_test(filename, "dynamic", dynamic);
        do_test(filename, "greedy", greedy);
    }
    else
    {
        fprintf(stderr, "File missing in inputs dyrectory.\n");
    }

    return 0;
}


