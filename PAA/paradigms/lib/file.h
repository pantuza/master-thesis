#ifndef FILE_H
#define FILE_H

#include <stdio.h>


#define FILENAME_SIZE 50
#define FILE_PERMISSION "r"
#define SUCCESS 0


FILE open_file(char filename[FILENAME_SIZE])
{
    FILE *file = fopen(filename, FILE_PERMISSION);

    if(file == NULL)
    {
        perror("Error during file open");
        exit(1);
    }

    return *file;
}

void close_file(FILE **file)
{
    int status = fclose(file);

    if(status != SUCCESS)
    {
        printf("Error during file close. FILE: %d\n", file);
        exit(2);
    }
}

#endif
