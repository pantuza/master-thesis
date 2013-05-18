#ifndef PPM_H
#define PPM_H

/* Size of magic string used in header of PPM image files */
#define MAGIC_STRING_SIZE 3

/* Token used by PPM format to identify comments inside a PPM file */
#define COMMENTS_TOKEN '#'

/* Maximum line size of a PPM file */
#define BUFFER_SIZE 71


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
    int heigh;
    int intensity;
    Pixel **pixels;
} PPMImage;


/* Functions */
void check_magic_string(FILE *, PPMImage *);
void ignore_garbage(FILE *, char *);
int get_number(FILE *);
void allocate_pixels(FILE *, PPMImage *);
PPMImage import(FILE *);
int export(PPMImage *);

#endif
