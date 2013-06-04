/**
 * tp2 program
 *
 * @author: Gustavo Pantuza
 * @since: 17.05.2013
 *
 */


#include <time.h>
#include <unistd.h>
#include <string.h>

#include "tp2.h"
#include "file.h"
#include "ppm.h"
#include "graph.h"
#include "dynamic.h"
#include "energy.h"


/* Time Function */
float timediff(clock_t end, clock_t start)
{
    return ((float) end - (float) start) / (float) CLOCKS_PER_SEC;
}


/**
 * Help message to the program execution. Prints the usage text information
 */
void usage()
{
    fprintf(stderr, 
            "Usage:\t./tp2 INPUT_FILE [OPTIONS]\n\n"
            "INPUT_FILE\n"
            "\tThe input file must be in ppm format. "
            "see: http://netpbm.sourceforge.net/doc/ppm.html\n\n"
	        "OPTIONS\n"
            "\t-g\t\tRun the graph solution\n"
            "\t-d\t\tRun the dynamic programming solution\n"
            "\t-w width\tResize image in width pixels\n"
            "\t-h height\tResize image in height pixels\n"
            "\t-m file\t\tResize image using the file with "
            "the weight matrices\n"
            "--\n");
    exit(EXIT_FAILURE);
}


/* Fill default options to the program */ 
void fill_default(Options *resize)
{
    // default value
    resize->method = DYNAMIC;
    resize->width = 0;
    resize->height = 0;
    resize->matrix = "in/sobel_weights";
}


/**
 * Command line options parser
 */
void opt_parser(int opt, Options *resize)
{
    fill_default(resize);

    switch(opt)
    {
        case 'g':
        case 'd':
            if(opt == 'g')
                resize->method = GRAPH;
            else
                resize->method = DYNAMIC;
            break;
        case 'w': 
            resize->width = atoi(optarg);
            break;
        case 'h': 
            resize->height = atoi(optarg);
            break;
        case 'm': 
            resize->matrix = optarg;
            break;
        case ':':
            fprintf(stderr, "missing argument from option -%c\n", optopt);
            usage();
            break;
        case '?':
            fprintf(stderr, "invalid option -%c\n", optopt);
            usage();
            break;
    }
}


/**
 * Uses GNU getopt to parse command line 
 * arguments and call opt_parser
 */
void arg_parser(int argc, char *argv[], Options *resize)
{
    int opt;
    char *options = ":gdw:h:m:";

    do
    {
        opt = getopt(argc, argv, options);
        opt_parser(opt, resize);

    } while(opt != -1);

    if (optind < argc)
        resize->ppmfile = argv[optind];
    else 
    {
        fprintf(stderr, "Missing input file\n\n");
        usage();
    }
}


/* Resize the image using graph or dynamic method */
void resize_image(PPMImage *image, Options *resize)
{
    if(resize->method == GRAPH)
        graph_resize(image, resize->width, resize->height);
    else 
        dynamic_resize(image, resize->width, resize->height);
}


int main(int argc, char *argv[])
{
    setvbuf(stdout, NULL, _IOLBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);

    // Program execution options
    Options resize;
    arg_parser(argc, argv, &resize);
    
    /* Timers */
    clock_t start;
    clock_t end;
    
    /* Files open */
    char in[1024]; sprintf(in, "in/%s", resize.ppmfile);
    char out[1024]; sprintf(out, "out/%s", resize.ppmfile);

    FILE *fileIn = openfile(in, READ_MODE);
    FILE *fileOut = openfile(out, WRITE_MODE);

    /* Imports input file */
    start = clock();
    PPMImage img = import(fileIn);
    end = clock();
    fprintf(stdout, "Import in %f seconds\n", timediff(end, start));

    /* Calculate the energy of image pixels */
    energise(&img, resize.matrix);

    /* Resize the image */
    resize_image(&img, &resize);
    
    /* exports to output file */
    start = clock();
    export(fileOut, &img);
    end = clock();
    fprintf(stdout, "Export in %f seconds\n", timediff(end, start));

    /* deallocate variables */
    free_pixels(&img);
    closefile(fileIn);
    closefile(fileOut);
    
    return EXIT_SUCCESS;
}
