/**
 * Solution to Maximum Flow problem
 *
 * author: Gustavo Pantuza
 * since: 13.06.2013
 */


#include <stdio.h>
#include <string.h>

#include "file.h"
#include "champ.h"
#include "tp3.h"
#include "flow.h"


/**
 * Prints how to usage the program
 */
void usage()
{
    fprintf(stderr, 
            "Usage: ./bin/tp3 <input-file> <output-file>\n");
    exit(EXIT_FAILURE);
}


/**
 * Parses the command line arguments and store the 
 * input and output files
 */ 
void arg_parser(int argc, char *argv[], char *input, char *output)
{
    if(argc == 3)
    {
        strcpy(input, argv[1]);
        strcpy(output, argv[2]);
    
    } else usage(); 
}


/**
 * Main execution of the program. Function calls
 * to load data from file, calculation of maximum flow 
 * and to save result answer to a file
 */ 
int main(int argc, char *argv[])
{
    /* Corrects terminal output buffer */
    setvbuf(stderr, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);

    
    /* Parse main arguments */
    char input[ARGS_BUFFER_SIZE], output[ARGS_BUFFER_SIZE];
    arg_parser(argc, argv, input, output);

#ifdef MYDEBUG
    printf("input: %s output: %s\n", input, output);
#endif


    /* Import input file */
    FILE *inpfile = openfile(input, READ_MODE); 

    /* Loads data from file */
    Champ champ;
    load(inpfile, &champ);
    closefile(inpfile);

    /* output file that will store the results */
    FILE *outfile = openfile(output, WRITE_MODE);


    /* Calculate the maximum flow on the graph */
    maximum_flow(&champ, outfile);
    closefile(outfile);
    
    return EXIT_SUCCESS;
}
