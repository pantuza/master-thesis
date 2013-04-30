#ifndef STR_TYPE_H
#define STR_TYPE_H

#include <stdio.h>
#include <stdlib.h>

#include <string.h>


#define WORD_SIZE 50


typedef struct
{
    char letters[WORD_SIZE];
    int current;
    int next;
} Word;

#endif
