#include <errno.h>
#include "file.h"
#include "strtype.h"

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
 * ignore rest of current line
 */
void next_line(FILE *file) {
    int c;
    do {
        c = fgetc(file);
    } while (c && c != '\n');
}
/*
 * read a integer (k) from input file (observing limits and security)
 */
int get_segment_size(FILE *file)
{
    // set a buffer with SEGMENT_SIZE_BUFFER (extra space for '\0')
    char buffer[SEGMENT_SIZE_BUFFER + 1];
    int number;
    fgets(buffer, SEGMENT_SIZE_BUFFER + 1, file);
    sscanf(buffer, "%s\n", buffer);
    // limit condition: ignore rest of current line
    if (strlen(buffer) == SEGMENT_SIZE_BUFFER) {
        next_line(file);
    }
    number = atoi(buffer);
    if (number < 0 || number > WORD_SIZE)
        return 0;
    return number;
}


/*
 * read a word from input file (observing limits and security)
 * return the size of read word
 * Notes:
 * handling alphabet error: discard all invalid characters
 * handling line too long error: discard all characters until end of line
 */
int get_word(FILE *file, char *word)
{
    // set a buffer with WORD_SIZE (extra space for '\0')
    char buffer[WORD_SIZE + 1];
    // empty the word
    word[0] = 0;
    fgets(buffer, WORD_SIZE + 1, file);
    // read only valid characters [a..z] from the buffer
    sscanf(buffer, "%[a-z]s\n", word);
    // force end of the word (in limit conditions)
    word[WORD_SIZE] = 0;
    int word_size = strlen(word);
    // limit condition: ignore rest of current line
    if (word_size == WORD_SIZE)
        next_line(file);
    return word_size;
}

/*
 * execute the method to calculate LCS for all input cases
 */
void sweep_input(FILE *file, method execute)
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
            // sort the arguments (words)
            //if (strlen(word0) <= strlen(word1))
                lcs = execute(segment_size, word0, size0, word1, size1);
            //else
            //    lcs = execute(segment_size, word1, size1, word0, size0);

            // outputs the calculated size of longest common subsequence
            printf("%d\n", lcs);
        }
        else
        {
            // returns zero, that always is a valid size of LCS,
            // even in this system error condition (empty line or word)
            printf("0");
        }

        // read another instance of k (zero implies end of execution)
        segment_size = get_segment_size(file);
    }
}

/*
 * close a file (observing error handling)
 */
void close_file(FILE *file)
{
    fclose(file);
    if(errno)
    {
        fprintf(stderr, "Error during file close.");
        exit(2);
    }
}
