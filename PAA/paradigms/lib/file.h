#ifndef FILE_H
#define FILE_H

#include <stdlib.h>
#include <stdio.h>

#define FILENAME_SIZE 1024
#define FILE_PERMISSION "r"

typedef int method(int , const char*, int , const char *, int);

FILE* open_file(char* filename);
void sweep_input(FILE *file, method* execute);
void close_file(FILE *file);

#endif
