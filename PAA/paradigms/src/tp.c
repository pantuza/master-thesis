/*
 * program for build executables for specific method
 */
#include <stdio.h>
#include <stdlib.h>
#include "strtype.h"
#include "file.h"
#include "dynamic.h"
#include "brute.h"
#include "greedy.h"

//#define METHOD brute_force
#define METHOD dynamic
/*
 * main
 */
int main(int argc, char** argv)
{
    char* filename = "01_test_input";
    FILE *file;
    file = open_file(filename);
    sweep_input(file, METHOD);
    close_file(file);

    return 0;
}
