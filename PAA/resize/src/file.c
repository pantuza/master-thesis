/**
 * File handler
 *
 * This file is responsible to manipulate files 
 * used in other programs
 *
 * @author: Gustavo Pantuza
 * @since: 17.05.2013
 *
 */

#include "file.h"


/**
 * Try to open a file. if occur an error, exits with a failure message
 */
FILE* openfile(char *filename)
{
    FILE *file = fopen(filename, FILE_MODE);

    if (file == NULL)
    {
        fprintf(stderr, "Error during file open: %s\n", filename);
        exit(EXIT_FAILURE);
    }
    return file;
}

/**
 * Try to close a file. if occur an error, exits with a failure message
 */ 
void closefile(FILE *file)
{
    fclose(file);
    if(errno)
    {
        fprintf(stderr, "Error during file close\n");
        exit(EXIT_FAILURE);
    }
}
