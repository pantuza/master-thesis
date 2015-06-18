#ifndef FILE_H
#define FILE_H

#include <stdlib.h>
#include <stdio.h>

#define MAX(a,b) ((a) > (b)? (a) : (b))

#define FILENAME_SIZE 1024
#define FILE_PERMISSION "r"

typedef int method(int , const char*, int, const char *, int);

FILE* open_file(char* filename);
int get_segment_size(FILE *);
int get_word(FILE *, char *);
void sweep_input(FILE *file, method* execute);
void close_file(FILE *file);

#endif
