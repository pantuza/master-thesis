#ifndef DYNAMIC_H
#define DYNAMIC_H

#include "ppm.h"
#include "file.h"

typedef struct Seam
{
    Energy total;
    int dx;
} Seam;


typedef struct SeamsStore
{
    int size;
    Seam **store;
} SeamsStore;


/* Public Functions */
void dynamic_resize(PPMImage *, int, int); 
#endif
