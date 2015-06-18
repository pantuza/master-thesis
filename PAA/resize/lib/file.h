/*
 * file.h
 *
 * @author: Gustavo Pantuza Coelho Pinto
 * @since: 17.05.2013
 *
 */
#ifndef FILE_H
#define FILE_H

#include <stdlib.h>
#include <stdio.h>

#define READ_MODE "r"
#define WRITE_MODE "w"


FILE* file_open(char *, char *mode);
void file_close(FILE *);

#endif
