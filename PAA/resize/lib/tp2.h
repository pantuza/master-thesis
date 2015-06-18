/*
 * tp2.h
 *
 * @author: Gustavo Pantuza Coelho Pinto
 * @since: 17.05.2013
 *
 */
#ifndef OPTIONS_H
#define OPTIONS_H


/* Allowed methods of exection */
enum methods
{
    // Based on graphs
    GRAPH,
    // Dynamic Programming aproach
    DYNAMIC
};


/* Command line options that configures the program exection */
typedef struct
{
    // Running Method
    enum methods method;
    // Width of the resized image
    int width;
    // Height of the resized image
    int height;
    // Input file with matrices to calculate pixel intensity
    char *matrix;
    // Input file with the image to be resized
    char *ppmfile;
    // Output file with the energised image
    char *energisedfile;
    // Output file with the resized image
    char *output;
    // Output file with the seam carving resized image
    char *outputPreview;
    // Execute correction test
    int correction_test;
    // Execute time analisys
    char *time_test_file;
} Options;

#endif

