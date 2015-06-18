/*
 * ppm.h
 *
 * @author: Gustavo Pantuza Coelho Pinto
 * @since: 17.05.2013
 *
 */
#ifndef PPM_H
#define PPM_H
#include <float.h>

#include "file.h"
#include "color.h"

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
#define ENERGY_MAX DBL_MAX

/* Utils */
#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b))

/* Struct representing one pixel with its RGB values */
typedef struct
{
    int R;
    int G;
    int B;
    int x, y;
    Energy energy;
} Pixel;


/* Struct representing one Image in the PPM format */
typedef struct
{
    int width;
    int height;
    int intensity;
    Pixel **pixels;
} PPMImage;


/* Public Functions */
PPMImage image_import(FILE *);
void image_export(FILE *, const PPMImage *);
void image_export_energy(FILE *, const PPMImage *);
void image_free(PPMImage *);
void image_copy(PPMImage *, const PPMImage *);
void image_remove_path(PPMImage *, int[]);
PPMImage image_new(int width, int height);
PPMImage image_new_from(const PPMImage *);
PPMImage image_new_transposed(const PPMImage *);
void image_copy_transposed(PPMImage *,  const PPMImage *);
void image_draw_path(const PPMImage *, const PPMImage *, int *, Color *);
void image_draw_tpath(const PPMImage *, const PPMImage *, int *, Color *);

#endif
