#ifndef FILE_H
#define FILE_H

#include <stdlib.h>
#include <stdio.h>

#define READ_MODE  0
#define WRITE_MODE 1


FILE* openfile(char *, int write);
void closefile(FILE *);

#endif
