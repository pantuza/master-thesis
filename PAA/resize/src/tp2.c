/**
 * tp2 program
 *
 * @author: Gustavo Pantuza
 * @since: 17.05.2013
 *
 */


#include <time.h>
#include <unistd.h>

#include "file.h"
#include "ppm.h"

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

void opt_parser(int opt)
{
    switch(opt)
    {
        case 'g': 
        case 'd': printf("-d\n"); break;
        case 'w': printf("-w: %s\n", optarg); break;
        case 'h': printf("-h: %s\n", optarg); break;
        case 'm': printf("-m: %s\n", optarg); break;
        case ':': printf("missing argument from option -%c\n", optopt); break;
        case '?': printf("invalid option -%c\n", optopt); usage(); break;
    }
}

void arg_parser(int argc, char *argv[])
{
    int opt;
    char *options = ":gdw:h:m:";
    extern char *optarg;
    extern int optopt;
    
    do
    {
        opt = getopt(argc, argv, options);
        opt_parser(opt);

    } while(opt != -1);

    if (optind < argc)
        printf ("ind: %d -> %s \n",optind, argv[optind]);
    else 
        printf("Missing input file");
}


int main(int argc, char *argv[])
{
    arg_parser(argc, argv);
    exit(0);
    clock_t start;
    clock_t end;
    FILE *fileIn = openfile("in/deathvalley.ppm", READ_MODE);
    FILE *fileOut = openfile("out/deathvalley.ppm", WRITE_MODE);

    start = clock();
    PPMImage img = import(fileIn);
    end = clock();
    printf("Import in %f seconds\n", ((float)end - (float)start) / (float)CLOCKS_PER_SEC);

    start = clock();
    export(fileOut, &img);
    end = clock();
    printf("Export in %f seconds\n", ((float)end - (float)start) / (float)CLOCKS_PER_SEC);

    free_pixels(&img);
    closefile(fileIn);
    closefile(fileOut);
    exit(EXIT_SUCCESS);
}
