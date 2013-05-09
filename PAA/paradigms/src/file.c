#include "file.h"

/*
 * open a file (observing error handling)
 */
FILE* open_file(char* filename)
{
    FILE *file = fopen(filename, FILE_PERMISSION);

    if(file == NULL)
    {
        fprintf(stderr, "Error during file open. FILE: %s\n", filename);
        exit(1);
    }

    return file;
}

/*
 * read a integer (k) from input file (observing limits and security)
 */
int get_segment_size(FILE *file)
{
    char k[SEGMENT_SIZE];
    int number;
    fgets(k, SEGMENT_SIZE, file);
    number = atoi(k);
    if (number < 0 || number > WORD_SIZE)
        return 0;
    return number;
}


/*
 * read a word from input file (observing limits and security) 
 */
void get_word(FILE *file, char *word)
{
    // set a buffer with WORD_SIZE + LF + CR
    char buffer[WORD_SIZE+2];
    fgets(buffer, WORD_SIZE, file);
    sscanf(buffer, "%s\n", word);
}

/*
 * execute the method to calculate LCS for all input cases
 */
void sweep_input(FILE *file, method execute)
{
    int lcs;
    char word0[WORD_SIZE], word1[WORD_SIZE];

    // read the first k (zero implies end of execution)
    int segment_size = get_segment_size(file);

    // while exist input case to execute (test case)
    while(segment_size != 0)
    {
        // reads both words
        get_word(file, word0);
        get_word(file, word1);

        // sort the arguments (words)
        if (strlen(word0) <= strlen(word1))
            lcs = execute(segment_size, word0, word1);
        else
            lcs = execute(segment_size, word1, word0);

        // outputs the calculated size of longest common subsequence
        printf("%d\n", lcs);

        // read another instance of k (zero implies end of execution)
        segment_size = get_segment_size(file);
    }
}

/*
 * close a file (observing error handling)
 */
void close_file(FILE *file)
{
    if(!fclose(file))
    {
        fprintf(stderr, "Error during file close.");
        exit(2);
    }
}
