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

#include <errno.h>
#include "file.h"

/**
 * Try to open a file. if occur an error, exits with a failure message
 */
FILE* file_open(char *filename, char *mode)
{
    FILE *file = fopen(filename, mode);

    if (file == NULL)
    {
        fprintf(stderr, "Error during file open in %s mode: %s\n",
                mode, filename);
        exit(EXIT_FAILURE);
    }
    return file;
}

/**
 * Try to close a file. if occur an error, exits with a failure message
 */ 
void file_close(FILE *file)
{
    fclose(file);
    if(errno)
    {
        fprintf(stderr, "Error during file close\n");
        exit(EXIT_FAILURE);
    }
}
