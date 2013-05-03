#ifndef FILE_H
#define FILE_H

#include <stdio.h>
#include <stdlib.h>

#include "strtype.h"

#define FILENAME_SIZE 50
#define FILE_PERMISSION "r"
#define SUCCESS 0


FILE open_file(char filename[FILENAME_SIZE]);
int get_seq_size(FILE *file);
Word word_handler(FILE *file);
void close_file(FILE *file);

#endif
