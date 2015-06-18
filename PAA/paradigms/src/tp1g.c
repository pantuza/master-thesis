/*
 * program for build executables for specific method
 */
#include <stdio.h>
#include <stdlib.h>
#include "file.h"
#include "greedy.h"

/*
 * main
 */
int main(int argc, char** argv)
{
    sweep_input(stdin, greedy);
    return 0;
}

