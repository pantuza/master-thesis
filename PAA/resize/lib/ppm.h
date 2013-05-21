#ifndef PPM_H
#define PPM_H

/* Size of magic string used in header of PPM image files */
#define MAGIC_STRING_SIZE 3

/* Token used by PPM format to identify comments inside a PPM file */
#define COMMENTS_TOKEN '#'

/* Maximum line size of a PPM file */
#define FILE_BUFFER_SIZE 8192

/* Maximum line size of a PPM number string */
#define NUMBER_BUFFER_SIZE 16


/* Struct representing one pixel with its RGB values */
typedef struct
{
    int R;
    int G;
    int B;
} Pixel;


/* Struct representing one Image in the PPM format */
typedef struct
{
    char magic_string[MAGIC_STRING_SIZE];
    int width;
    int height;
    int intensity;
    Pixel **pixels;
} PPMImage;


/* Public Functions */
PPMImage import(FILE *, FILE *);
void export(FILE *, PPMImage *);
void allocate_pixels(PPMImage *);
void free_pixels(PPMImage *);

#endif
