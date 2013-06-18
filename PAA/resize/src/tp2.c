/**
 * tp2 program
 *
 * @author: Gustavo Pantuza Coelho Pinto
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
#include "resize.h"


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
    resize->matrix = NULL;
    resize->energisedfile = NULL;
    resize->ppmfile = NULL;
    resize->output = NULL;
    resize->outputPreview = NULL;
    resize->test = 0;
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
        case 't':
            resize->test = 1;
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
    char *options = ":gdw:h:m:e:o:p:t";

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
PPMImage resize_image(PPMImage *image, Options *opt)
{
    // convert negative value in percentage reduction
    if (opt->width < 0)
    {
        opt->width = (int)((float)image->width *
                (float)((float)(abs(opt->width)) / 100.0));
    }
    if (opt->height < 0)
    {
        opt->height = (int)((float)image->height *
                (float)((float)(abs(opt->height)) / 100.0));
    }
    //
    if (opt->test)
        return image_resize_compare(dp_shortest_path, graph_shortest_path,
                image, opt->width, opt->height);
    else
    {
        DEBUG(fprintf(stderr,"method: %d\n", opt->method));
        ShortestPath *method;
        if(opt->method == GRAPH)
        {
            INFO(fprintf(stderr, "Method: Dijkstra\n"));
            method = &graph_shortest_path;
        }
        else
        {
            INFO(fprintf(stderr, "Method: Dynamic programming\n"));
            method = &dp_shortest_path;
        }
        if (opt->outputPreview == NULL)
            return image_resize(*method, image, opt->width, opt->height);
        else
            return image_resize_preview(*method, image, opt->width, opt->height);
    }
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
    FILE *fileIn = file_open(opt.ppmfile, READ_MODE);
    PPMImage img = image_import(fileIn);
    file_close(fileIn);
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
        FILE *fileGray = file_open(opt.energisedfile, WRITE_MODE);
        image_export_energy(fileGray, &img);
        file_close(fileGray);
        end = clock();
        INFO(fprintf(stderr, "Export energised image in %f seconds\n",
                timediff(end, start)));
    }

    /* Resize (calculation) the image */
    start = clock();
    PPMImage resized = resize_image(&img, &opt);
    end = clock();
    INFO(fprintf(stderr, "Resize (calculation) image in %f seconds\n",
            timediff(end, start)));

    /* exports resized image to output file */
    start = clock();
    if (opt.output != NULL)
    {
        FILE *fileOut = file_open(opt.output, WRITE_MODE);
        image_export(fileOut, &resized);
        file_close(fileOut);
    }
    else
        image_export(stdout, &resized);
    end = clock();
    INFO(fprintf(stderr, "Export resized in %f seconds\n",
            timediff(end, start)));

    /* exports preview to output file */
    if (opt.outputPreview != NULL)
    {
        start = clock();
        FILE *fileOut = file_open(opt.outputPreview, WRITE_MODE);
        image_export(fileOut, &img);
        file_close(fileOut);
        end = clock();
        INFO(fprintf(stderr, "Export preview in %f seconds\n",
                timediff(end, start)));
    }

    /* deallocate variables */
    image_free(&img);
    image_free(&resized);
    
    return EXIT_SUCCESS;
}
