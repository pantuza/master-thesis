#include "file.h"

FILE* open_file(char filename[FILENAME_SIZE])
{
    FILE *file = fopen(filename, FILE_PERMISSION);

    if(file == NULL)
    {
        perror("Error during file open");
        exit(1);
    }

    return file;
}


int get_seq_size(FILE *file)
{
    char min_size[WORD_SIZE];
    fgets(min_size, sizeof min_size, file);
    return atoi(min_size);
}


Word word_handler(FILE *file)
{
    Word word;
    fgets(word.letters, WORD_SIZE, file);
    word.current = 0;
    word.next = 1;
    return word;
}


void close_file(FILE *file)
{
    int status = fclose(file);

    if(status != SUCCESS)
    {
        printf("Error during file close. FILE: %d\n", file);
        exit(2);
    }
}
