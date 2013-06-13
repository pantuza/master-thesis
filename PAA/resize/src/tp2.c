/**
 * tp2 program
 *
 * @author: Gustavo Pantuza
 * @since: 17.05.2013
 *
 */


#include <time.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>

#include "tp2.h"
#include "file.h"
#include "ppm.h"
#include "graph.h"
#include "dynamic.h"
#include "energy.h"
#include "debug.h"


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
            "\t-o file\t\tRedirect stdout to file (test)\n"
            "\t-o file\t\tSave image preview to file (test)\n"
            "\t-e file\t\tSave energised image (test)\n"
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
    resize->energisedfile = NULL;
    resize->ppmfile = NULL;
    resize->output = NULL;
    resize->outputPreview = NULL;
}


/**
 * Command line options parser
 */
void opt_parser(int opt, Options *resize)
{
    switch(opt)
    {
        case 'g':
            resize->method = GRAPH;
            break;
        case 'd':
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
        case 'e':
            resize->energisedfile = optarg;
            break;
        case 'o':
            resize->output = optarg;
            break;
        case 'p':
            resize->outputPreview = optarg;
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
    char *options = ":gdw:h:m:e:o:p:";

    fill_default(resize);

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
    DEBUG(fprintf(stderr,"method: %d\n", resize->method));
    if(resize->method == GRAPH)
        graph_resize(image, resize->width, resize->height);
    else 
        dynamic_resize(image, resize->width, resize->height);
}


int main(int argc, char *argv[])
{
    //setvbuf(stdout, NULL, _IOFBF, FILE_BUFFER_SIZE);
    setvbuf(stderr, NULL, _IONBF, 0);

    // Program execution options
    Options opt;
    arg_parser(argc, argv, &opt);
    
    /* Timers */
    clock_t start;
    clock_t end;
    
    /* Files open */

    /* Imports input file */
    start = clock();
    FILE *fileIn = openfile(opt.ppmfile, READ_MODE);
    PPMImage img = import(fileIn);
    closefile(fileIn);
    end = clock();
    INFO(fprintf(stderr, "Import in %f seconds\n", timediff(end, start)));

    /* Calculate the energy of image pixels */
    start = clock();
    // Meanwhile, at stardate 5906.4, in transporter room...
    // KIRK: All right, Mister Scott, energise.
    energise(&img, opt.matrix);
    end = clock();
    INFO(fprintf(stderr, "Energy calculation in %f seconds\n",
            timediff(end, start)));

    /* exports grayscale to output file */
    if (opt.energisedfile != NULL)
    {
        start = clock();
        FILE *fileGray = openfile(opt.energisedfile, WRITE_MODE);
        export_energy(fileGray, &img);
        closefile(fileGray);
        end = clock();
        INFO(fprintf(stderr, "Export energised image in %f seconds\n",
                timediff(end, start)));
    }

    /* Resize (calculation) the image */
    start = clock();
    resize_image(&img, &opt);
    end = clock();
    INFO(fprintf(stderr, "Resize (calculation) image in %f seconds\n",
            timediff(end, start)));

    /* Resize (copy) image  */
    start = clock();
    PPMImage newImg = resize(&img, opt.width, opt.height);
    end = clock();
    INFO(fprintf(stderr, "Resize (copy) image in %f seconds\n",
            timediff(end, start)));
    
    /* exports resized image to output file */
    start = clock();
    if (opt.output != NULL)
    {
        FILE *fileOut = openfile(opt.output, WRITE_MODE);
        export(fileOut, &newImg);
        closefile(fileOut);
    }
    else
        export(stdout, &newImg);
    end = clock();
    INFO(fprintf(stderr, "Export resized in %f seconds\n",
            timediff(end, start)));

    /* exports preview to output file */
    if (opt.outputPreview != NULL)
    {
        start = clock();
        FILE *fileOut = openfile(opt.outputPreview, WRITE_MODE);
        export(fileOut, &img);
        closefile(fileOut);
        end = clock();
        INFO(fprintf(stderr, "Export preview in %f seconds\n",
                timediff(end, start)));
    }

    /* deallocate variables */
    free_pixels(&img);
    free_pixels(&newImg);
    
    return EXIT_SUCCESS;
}
