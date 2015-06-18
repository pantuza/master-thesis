#ifndef FLOW_H
#define FLOW_H

#include <stdio.h>

#include "champ.h"


#define WHITE 0
#define GRAY 1
#define BLACK 2


#define MIN(x, y) ( ((x) < (y)) ? (x) : (y) )


int **capacity;
int **flow;
int *pred;


/* Public functions */
int ford_fulkerson(int, int);
void maximum_flow(Champ *, FILE *);
#endif
