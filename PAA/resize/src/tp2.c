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
    usage();
    exit(EXIT_SUCCESS);
}
