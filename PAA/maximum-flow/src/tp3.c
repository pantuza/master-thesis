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

#include "debug.h"


void usage()
{
    fprintf(stderr, 
            "Usage: ./bin/tp3 <input-file> <output-file>\n");
    exit(EXIT_FAILURE);
}

void arg_parser(int argc, char *argv[], char *input, char *output)
{
    if(argc == 3)
    {
        strcpy(input, argv[1]);
        strcpy(output, argv[2]);
    
    } else usage(); 
}


int main(int argc, char *argv[])
{
    /* Corrects terminal output buffer */
    setvbuf(stderr, NULL, _IONBF, 0);

    
    /* Parse main arguments */
    char input[ARGS_BUFFER_SIZE], output[ARGS_BUFFER_SIZE];
    arg_parser(argc, argv, input, output);
    DEBUG(printf("input: %s output: %s\n", input, output));


    /* Import input file */
    FILE *inpfile = openfile(input, READ_MODE); 

    Champ champ;
    load(inpfile, &champ);

    maximum_flow(&champ);



    /* Export output file */
    FILE *outfile = openfile(output, WRITE_MODE);




    return EXIT_SUCCESS;
}
