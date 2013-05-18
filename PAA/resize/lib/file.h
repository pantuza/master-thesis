#ifndef FILE_H
#define FILE_H

#include <stdlib.h>
#include <stdio.h>

#define FILE_MODE 'r'


FILE* openfile(char *);
void closefile(FILE *);

#endif
