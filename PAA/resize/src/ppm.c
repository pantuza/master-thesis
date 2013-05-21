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
#include <ctype.h>

#include "file.h"
#include "ppm.h"

/**
 * handle the read PPM file error
 * reports an error and exit
 */
void read_error()
{
    fprintf(stderr, "Invalid PPM format\n");
    fflush(stdout);
    exit(EXIT_FAILURE);
}

/*
 * Allocate memory, sets buffer and position control
*/
void init_buffer(char **buffer, int *pos)
{
    *buffer = (char *)(malloc(FILE_BUFFER_SIZE));
    *buffer[0] = EOF;
    *pos = FILE_BUFFER_SIZE + 1;
}

/*
 * Deallocated memory and nullifies the buffer
*/
void free_buffer(char **buffer)
{
    free(*buffer);
    *buffer = NULL;
}

/*
 * Read a character from file buffer
 * fread() version
*/
inline static char bgetc(FILE *file, char *buffer, int *pos)
{
    if (*pos >= FILE_BUFFER_SIZE)
    {
        int count = fread(buffer, sizeof(char), FILE_BUFFER_SIZE, file);
        if (count < FILE_BUFFER_SIZE)
            buffer[count] = EOF;
        *pos = 1;
        return buffer[0];
    }
    return buffer[(*pos)++];
}

/*
 * Read a character from file buffer
 * fgetc() version, that ignores the buffer
 * (seems that still uses file buffer from stdio).
 */
/*
inline static char bgetc(FILE *file, char *buffer, int *pos)
{
    return fgetc(file);
}
*/

/**
 * Ignore the unnecessary stuff brought from the file
 */
int get_number(FILE *file, char *buffer, int *pos)
{
    char c;
    c = bgetc(file, buffer, pos);
    if(c == EOF) read_error();
    // sapce = (' '|'\n'|'\r'|'\f'|'\t'|'\v')
    // (('#'*'\n')|space)*['0'..'9']*

    // parse (('#'*'\n')|space)*
    while(1)
    {
        // ignore (#*\n)
        if (c == COMMENTS_TOKEN)
        {
            while (c != '\n')
            {
                c = bgetc(file, buffer, pos);
                if(c == EOF) read_error();
            };
        }
        // ignore (space)
        if (isspace(c))
        {
            c = bgetc(file, buffer, pos);
            if(c == EOF) read_error();
        }
        else
            if (c < '0' || c > '9')
                // not a number!
                read_error();
            else
                break;
    }
    // parse ['0'..'9']*
    int i = 0;
    char number[NUMBER_BUFFER_SIZE];
    while (c >= '0' && c <= '9')
    {
        number[i++] = c;
        // is a too big number?
        if (i >= NUMBER_BUFFER_SIZE)
            read_error();
        else
        {
            c = bgetc(file, buffer, pos);
            if(c == EOF) break;
        }
    }
    number[i] = 0;
    return atoi(number);
}
/**
 * Verifies the PPM header. If the header is invalid
 * reports an error
 */
void get_magic_string(FILE *file, PPMImage *image)
{
    fgets(image->magic_string, MAGIC_STRING_SIZE, file);

    /* Plain text files must have the 'P3' magic string as a header */
    if(image->magic_string[0] != 'P' || image->magic_string[1] != '3')
    {
        fprintf(stderr, "Invalid PPM magic string\n");
        exit(EXIT_FAILURE);
    }
}

/**
 * Allocates the pixels matrix. The allocation uses malloc to create
 * the entire storage of the matrix based in its dimensions read from
 * the PPMImage struct. After memory allocation, it updates the pointers
 * references and then fill the chunks with values read from file
 */
void allocate_pixels(PPMImage *image)
{
    int first_dimension = image->width * sizeof(int *);
    int second_dimension = (image->width * image->height) * sizeof(Pixel);

    // Allocate consecutive memory
    image->pixels = (Pixel **)(malloc(first_dimension + second_dimension));
   
    if (image->pixels == NULL)
    {
        fprintf(stderr, "Image allocation failure.\n");
        exit(EXIT_FAILURE);
    }

    /* First data line (modifying pointers) */
    image->pixels[0] = (Pixel *)(image->pixels + image->width);

    /* Subsequent data lines (modifying pointers) */
    for(int i = 1; i < image->width; i++)
        image->pixels[i] = (Pixel *)(image->pixels[i-1] + image->height);

}

/**
 * Deallocates the pixels matrix.
 */
void free_pixels(PPMImage *image)
{
    // Deallocate memory
    free(image->pixels);
    image->pixels = NULL;
}

/**
 * Imports PPM image files and construct 
 * PPMImage struct.
 */
PPMImage import(FILE *file, FILE *file2)
{
    char *buffer;
    int pos;
    PPMImage image;

    init_buffer(&buffer, &pos);

    get_magic_string(file, &image);
    image.width = get_number(file, buffer, &pos);
    image.height = get_number(file, buffer, &pos);
    image.intensity = get_number(file, buffer, &pos);
    allocate_pixels(&image);

    /* fillind the data from file */
    for(int i = 0; i < image.width; i++)
        for(int j = 0; j < image.height; j++)
        {
            image.pixels[i][j].R = get_number(file, buffer, &pos);;
            image.pixels[i][j].G = get_number(file, buffer, &pos);;
            image.pixels[i][j].B = get_number(file, buffer, &pos);;
        }

    free_buffer(&buffer);

    return image;
}

/**
 * Exports a PPMImage to a file with PPM format
 */
void export(FILE *file, PPMImage *image)
{
    fprintf(file, "%s\n", image->magic_string);
    fprintf(file, "# image dimensions\n");
    fprintf(file, "%d %d\n", image->width, image->height);
    fprintf(file, "# image intensity\n");
    fprintf(file, "%d\n", image->intensity);
    fprintf(file, "# image data\n");
    /* writing the data to file */
    Pixel **pixels = image->pixels;
    for(int i = 0; i < image->width; i++)
    {
        fprintf(file, "# line %d\n", i);
        for(int j = 0; j < image->height; j++)
        {
            fprintf(file, "%d %d %d\n",
                    pixels[i][j].R,
                    pixels[i][j].G,
                    pixels[i][j].B);
        }
    }
}

