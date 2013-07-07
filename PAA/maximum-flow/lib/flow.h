#ifndef FLOW_H
#define FLOW_H

#include "champ.h"

#define WHITE 0
#define GRAY 1
#define BLACK 2

#define MIN(x, y) ( ((x) < (y)) ? (x) : (y) )


int **capacity;
int **flow;
int *pred;

/* Edge type */
typedef struct Edge
{
    int source;
    int dive;
    int capacity;
} Edge;

typedef struct Graph
{
    Edge * edges;
} Graph;


/* Public functions */
int ford_fulkerson(int, int);
void maximum_flow(Champ *);
#endif
