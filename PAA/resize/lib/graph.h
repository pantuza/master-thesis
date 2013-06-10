#ifndef GRAPH_H
#define GRAPH_H

#include "ppm.h"
#include "file.h"


/* Struct of the Adjancency list  */
typedef struct
{
    int vertex;
    double energy;
    void *next;
} Edge;


/* Struct of a Vertex */
typedef struct 
{
    double energy;
    Edge *edge;
} Vertex;


/* Struct of the entire Graph */
typedef struct
{
    int list_size;
    Vertex *vertexes;
} Graph;


/* Public Functions */
void graph_resize(PPMImage *, int, int);
void init_graph(Graph *, PPMImage *);
#endif
