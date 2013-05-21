#ifndef FILE_H
#define FILE_H

#include <stdlib.h>
#include <stdio.h>

#define READ_MODE "r"
#define WRITE_MODE "w"


FILE* openfile(char *, char *mode);
void closefile(FILE *);

#endif
