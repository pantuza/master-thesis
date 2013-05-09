#ifndef FILE_H
#define FILE_H

#include <stdlib.h>
#include <stdio.h>
#include "strtype.h"

#define FILENAME_SIZE 1024
#define FILE_PERMISSION "r"

typedef int method(int, char*, char*);

FILE* open_file(char* filename);
//int get_segment_size(FILE *file);
//void get_word(FILE *file, char* word);
void sweep_input(FILE *file, method* execute);
void close_file(FILE *file);

#endif
