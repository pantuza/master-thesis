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

#include "ppm.h"
#include "debug.h"
/**
 * handle the read PPM file error
 * reports an error and exit
 */
inline static void read_error()
{
    fprintf(stderr, "Invalid PPM format\n");
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

/**
 * Ignore comments inside the file
 */
inline static void ignore_comments(char *c, FILE *file, char *buffer, int *pos)
{
    while (*c != '\n')
    {
        *c = bgetc(file, buffer, pos);
        if(*c == EOF) 
            read_error();
    }
}

/**
 * Verifies if an unexpected EOF was found
 */
inline static void check_EOF(char *c, FILE *file, char *buffer, int *pos)
{
    *c = bgetc(file, buffer, pos);
    if(*c == EOF) 
        read_error();
}

/**
 * Matches a number inside the file. If the number is valid, 
 * it will be returned
 */
inline static int match_number(char *c, FILE *file, char *buffer, int *pos)
{
    // parse ['0'..'9']*
    int i = 0;
    char number[NUMBER_BUFFER_SIZE];
    while (*c >= '0' && *c <= '9')
    {
        number[i++] = *c;
        // is a too big number?
        if (i >= NUMBER_BUFFER_SIZE)
            read_error();
        else
        {
            *c = bgetc(file, buffer, pos);
            if(*c == EOF) break;
        }
    }
    number[i] = 0;
    return atoi(number);
}

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
            ignore_comments(&c, file, buffer, pos); 

        // ignore (space)
        if (isspace(c))
            check_EOF(&c, file, buffer, pos);

        else
            if (c < '0' || c > '9')
                // not a number!
                read_error();
            else
                break;
    }
    
    return match_number(&c, file, buffer, pos);
}

/**
 * Verifies the PPM header. If the header is invalid
 * reports an error
 */
void get_magic_string(FILE *file, PPMImage *image)
{
    char magic_string[MAGIC_STRING_SIZE];
    fgets(magic_string, MAGIC_STRING_SIZE, file);

    /* Plain text files must have the 'P3' magic string as a header */
    if(magic_string[0] != 'P' || magic_string[1] != '3')
    {
        fprintf(stderr, "Invalid PPM magic string\n");
        exit(EXIT_FAILURE);
    }
}


inline static int image_copy_must_realloc(const PPMImage *d, const PPMImage *s)
{
    return ((d->width < s->width) || (d->height < s->height));
}

inline static int image_copy_transposed_must_realloc(
        const PPMImage *d, const PPMImage *s)
{
    return ((d->width < s->height) || (d->height < s->width));
}

/**
 * Allocates the pixels matrix. The allocation uses malloc to create
 * the entire storage of the matrix based in its dimensions read from
 * the PPMImage struct.
 */
Pixel **allocate_pixels(const int width, const int height)
{
    int first_dimension = width * sizeof(Pixel *);
    int second_dimension = (width * height) * sizeof(Pixel);

    // Allocate consecutive memory
    Pixel **pixels = (Pixel **)(malloc(first_dimension + second_dimension));
   
    if (pixels == NULL)
    {
        fprintf(stderr, "Image allocation failure.\n");
        exit(EXIT_FAILURE);
    }

    /* First data line (modifying pointers) */
    pixels[0] = (Pixel *)(pixels + width);

    /* Subsequent data lines (modifying pointers) */
    for(int x = 1; x < width; x++)
        pixels[x] = (Pixel *)(pixels[x-1] + height);

    return pixels;

}

/**
 * Deallocates the pixels matrix.
 */
void image_free(PPMImage *image)
{
    // Deallocate memory
    free(image->pixels);
    image->pixels = NULL;
    image->height = 0;
    image->height = 0;
}

/**
 * fill the pixels data from file 
 */
void fill_pixels_data(FILE *file, char *buffer, int *pos, PPMImage *image)
{
    for(int y = 0; y < image->height; y++)
        for(int x = 0; x < image->width; x++)
        {
            image->pixels[x][y].R = get_number(file, buffer, pos);
            image->pixels[x][y].G = get_number(file, buffer, pos);
            image->pixels[x][y].B = get_number(file, buffer, pos);
            image->pixels[x][y].x = x;
            image->pixels[x][y].y = y;
        }
}

/**
 * Imports PPM image files and construct 
 * PPMImage struct.
 */
PPMImage image_import(FILE *file)
{
    char *buffer;
    int pos;
    PPMImage image;

    init_buffer(&buffer, &pos);

    get_magic_string(file, &image);
    image.width = get_number(file, buffer, &pos);
    image.height = get_number(file, buffer, &pos);
    image.intensity = get_number(file, buffer, &pos);
    image.pixels = allocate_pixels(image.width, image.height);

    fill_pixels_data(file, buffer, &pos, &image);

    free_buffer(&buffer);

    return image;
}

/**
 * Exports a PPMImage to a file with PPM format
 */
void image_export(FILE *file, const PPMImage *image)
{
    fprintf(file, "%s\n", "P3");
    fprintf(file, "# image dimensions\n");
    fprintf(file, "%d %d\n", image->width, image->height);
    fprintf(file, "# image intensity\n");
    fprintf(file, "%d\n", image->intensity);
    fprintf(file, "# image data\n");
    /* writing the data to file */
    Pixel **pixels = image->pixels;
    for(int y = 0; y < image->height; y++)
    {
        fprintf(file, "# line %d\n", y);
        for(int x = 0; x < image->width; x++)
        {
            fprintf(file, "%d %d %d \n",
                    pixels[x][y].R,
                    pixels[x][y].G,
                    pixels[x][y].B);
        }
    }
    fprintf(file, "\n");
}

/**
 * Exports a PGMImage to a file with PGM format
 * PGM is a grayscale image format
 * This is useful to check the energy function
 */
void image_export_energy(FILE *file, const PPMImage *image)
{
    int gray;
    fprintf(file, "P2\n");
    fprintf(file, "# image dimensions\n");
    fprintf(file, "%d %d\n", image->width, image->height);
    fprintf(file, "# image intensity\n");
    fprintf(file, "%d\n", image->intensity);
    fprintf(file, "# image data\n");
    /* writing the data to file */
    Pixel **pixels = image->pixels;
    int max = image->intensity;
    for(int y = 0; y < image->height; y++)
    {
        fprintf(file, "# line %d\n", y);
        for(int x = 0; x < image->width; x++)
        {
            gray = (int)(pixels[x][y].energy);
            if (gray > max)
                gray = max;
            fprintf(file, "%d\n", gray);
        }
    }
}

void image_remove_path(PPMImage *image, int path[])
{
    for(int y = 0; y < image->height; y++)
    {
        for(int x = path[y]; x < image->width - 1; x++)
            image->pixels[x][y] = image->pixels[x+1][y];
    }
    (image->width)--;
}

void image_draw_path(const PPMImage *original, const PPMImage *image,
        int *path, Color *color)
{
    int x;
    for(int y = 0; y < image->height; y++)
    {
        x = image->pixels[path[y]][y].x;
        Pixel * p = &(original->pixels[x][y]);
        p->R = color->r;
        p->G = color->g;
        p->B = color->b;
    }
#ifdef OPT_IMAGE_SEAM_DIFF_COLORS
    color_next(color);
#endif
}

void image_draw_tpath(const PPMImage *original, const PPMImage *image,
        int *path, Color *color)
{
    int x;
    for(int y = 0; y < image->height; y++)
    {
        x = image->pixels[path[y]][y].x;
        Pixel * p = &(original->pixels[y][x]);
        p->R = color->r;
        p->G = color->g;
        p->B = color->b;
    }
#ifdef OPT_IMAGE_SEAM_DIFF_COLORS
    color_next(color);
#endif
}

void image_copy(PPMImage *image, const PPMImage *source)
{
    if (image_copy_must_realloc(image, source))
    {
        if (image->pixels != NULL)
                free(image->pixels);
        image->pixels = allocate_pixels(source->width, source->height);
    }
    image->intensity = source->intensity;
    image->height = source->height;
    image->width = source->width;
    for(int x = 0; x < source->width; x++)
        for(int y = 0; y < source->height; y++)
            image->pixels[x][y] = source->pixels[x][y];
}

PPMImage image_new(int width, int height)
{
    PPMImage image;
    image.intensity = 255;
    image.height = height;
    image.width = width;
    image.pixels = allocate_pixels(width, height);
    return image;
}

PPMImage image_new_from(const PPMImage *source)
{
    PPMImage image = image_new(source->width, source->height);
    for(int x = 0; x < source->width; x++)
        for(int y = 0; y < source->height; y++)
            image.pixels[x][y] = source->pixels[x][y];
    return image;
}

PPMImage image_new_transposed(const PPMImage *source)
{
    PPMImage image = image_new(source->height, source->width);
    image.intensity = source->intensity;
    image.width = source->height;
    image.height = source->width;
    image.pixels = allocate_pixels(source->height, source->width);
    int x, y;
    for (x = 0; x < source->width; x++)
        for (y = 0; y < source->height; y++)
            image.pixels[y][x] = source->pixels[x][y];
    return image;
}

void image_copy_transposed(PPMImage *image,  const PPMImage *source)
{
    ASSERT_TRUE(image != source,
            fprintf(stderr, "ERROR: Impossible copy from same image.\n"); return;)
    if (image_copy_transposed_must_realloc(image, source))
    {
        if (image->pixels != NULL)
                free(image->pixels);
        image->pixels = allocate_pixels(source->height, source->width);
    }
    image->intensity = source->intensity;
    image->height = source->width;
    image->width = source->height;
    for(int x = 0; x < source->width; x++)
        for(int y = 0; y < source->height; y++)
            image->pixels[y][x] = source->pixels[x][y];
}


