/**
 * tp2 program
 *
 * @author: Gustavo Pantuza
 * @since: 17.05.2013
 *
 */#include <time.h>

#include "file.h"
#include "ppm.h"

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

int main()
{
    clock_t start;
    clock_t end;
    FILE *fileIn = openfile("in/deathvalley.ppm", READ_MODE);
    FILE *fileOut = openfile("out/deathvalley.ppm", WRITE_MODE);

    start = clock();
    PPMImage img = import(fileIn, fileOut);
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
