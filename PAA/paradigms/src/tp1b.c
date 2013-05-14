/*
 * program for build executables for specific method
 */
#include <stdio.h>
#include <stdlib.h>
#include "file.h"
#include "brute.h"

/*
 * main
 */
int main(int argc, char** argv)
{
    sweep_input(stdin, brute_force);
    return 0;
}

