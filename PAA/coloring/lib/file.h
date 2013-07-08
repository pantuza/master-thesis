#ifndef FILE_H
#define FILE_H

#include <stdlib.h>
#include <stdio.h>

#define READ_MODE   'r'
#define WRITE_MODE  'w'
#define APPEND_MODE 'a'


FILE* openfile(char *, char);
void closefile(FILE *);

#endif
