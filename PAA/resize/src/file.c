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


FILE* openfile(char *filename)
{
    FILE *file = fopen(filename, FILE_MODE);

    if (file == NULL)
    {
        fprintf(stderr, "Error during file open: %s\n", filename);
        exit(1);
    }
    return file;
}

void closefile(FILE *file)
{
    fclose(file);
    if(errno)
    {
        fprintf(stderr, "Error during file close\n");
        exit(2);
    }
}
