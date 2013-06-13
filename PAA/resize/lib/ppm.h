#ifndef PPM_H
#define PPM_H

#include "file.h"

/* Size of magic string used in header of PPM image files */
#define MAGIC_STRING_SIZE 3

/* Token used by PPM format to identify comments inside a PPM file */
#define COMMENTS_TOKEN '#'

/* Maximum line size of a PPM file */
#define FILE_BUFFER_SIZE 8192

/* Maximum line size of a PPM number string */
#define NUMBER_BUFFER_SIZE 16

/* Energy type (float or double?)
 * http://docs.oracle.com/cd/E19957-01/806-3568/ncg_goldberg.html
 */
typedef double Energy;
#define ENERGY_FORMAT "%lf"

/* Struct representing one pixel with its RGB values */
typedef struct
{
    int R;
    int G;
    int B;
    Energy energy;
} Pixel;


/* Struct representing one Image in the PPM format */
typedef struct
{
    char magic_string[MAGIC_STRING_SIZE];
    int width;
    int height;
    int intensity;
    Energy energy;
    Pixel **pixels;
} PPMImage;


/* Public Functions */

void init_buffer(char **, int *);
void free_buffer(char **);

void allocate_pixels(PPMImage *);
void free_pixels(PPMImage *);
void fill_pixels_data(FILE *, char *, int *, PPMImage *);

PPMImage import(FILE *);
void export(FILE *, PPMImage *);
void export_energy(FILE *, PPMImage *);

PPMImage resize(PPMImage *, int, int);

#endif
