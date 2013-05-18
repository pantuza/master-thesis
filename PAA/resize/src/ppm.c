/**
 * This is the PPM parser.
 * 
 * The PPM format is a lowest common denominator color image file format
 * documentation: http://netpbm.sourceforge.net/doc/ppm.html
 *
 * PPM format is simpler than JPEG and PNG. It describes the image
 * as a text file. 
 *
 * This file is a PPM parser to import and export PPM files.
 *
 * @author: Gustavo Pantuza
 * @since: 17.05.2013
 *
 */

#include <string.h>

#include "ppm.h"
#include "file.h"


/**
 * Verifies the PPM header. If the header is invalid
 * reports an error
 */
void check_magic_string(FILE *file, PPMImage *image)
{
    fgets(image->magic_string, MAGIC_STRING_SIZE, file);

    /* Plain text files must have the 'P3' magic string as a header */
    if(image->magic_string[0] != 'P' || image->magic_string[1] != '3')
    {
        fprintf(stderr, "Invalid PPM file");
        exit(EXIT_FAILURE);
    }
}

/**
 * Ignore the unnecessary stuff brought from the file
 */
void ignore_garbage(FILE *file, char *buffer) 
{
    while(*buffer != COMMENTS_TOKEN)
    {
        if(!*buffer) return;
        buffer++;
    }
    while(*buffer)
    {
        if(*buffer == '\n') return;
        buffer++;
    }

    int c = fgetc(file);
    while (c && c != '\n' && c != EOF);
        c = fgetc(file);
}

/**
 * Get a number from file
 */
int get_number(FILE *file)
{
    char buffer[BUFFER_SIZE];
    fgets(buffer, BUFFER_SIZE, file);
    ignore_garbage(file, buffer);
    return atoi(buffer);
}

/**
 * Allocates the pixels matrix. The allocation uses malloc to create
 * the entire storage of the matrix based in its dimensions read from
 * the PPMImage struct. After memory allocation, it updates the pointers
 * references and then fill the chunks with values read from file
 */
void allocate_pixels(FILE *file, PPMImage *image)
{
    int first_dimension = image.width * sizeof(int *);
    int second_dimension = (image.width * image.height) * sizeof(Pixel);

    image->pixels = (int **) malloc(first_dimension + second_dimension);
   
    /* First data line (modifying pointers) */
    image.pixels[0] = image.pixels + image.width;

    /* Subsequent data lines (modifying pointers) */
    for(int i=1; i < image.width; i++)
        image.pixels[i] = image.pixels[i-1] + image.height * sizeof(Pixel);

    /* fillind the data from file */
    for(int i = 0; i < image.width; i++)
        for(int j = 0; j < image.height; j++)
            image.pixels[i][j] = get_number(file);
}
/**
 * Imports PPM image files and construct 
 * PPMImage struct.
 */
PPMImage import(FILE *file)
{
    PPMImage image;
    char buffer[BUFFER_SIZE];

    check_magic_string(file, &image);
    image.width = get_number(file);
    image.height = get_number(file);
    image.intensity = get_number(file);
    allocate_pixels(file, &image);

    return image;
}

/**
 * Exports a PPMImage to a file with PPM format
 */
int export(Image *image)
{

}
