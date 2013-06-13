#ifndef GRAPH_H
#define GRAPH_H

#include "ppm.h"
#include "file.h"

/* Struct of a Vertex */
typedef struct Vertex
{
        Pixel *pixel;
        int adj[4];
} Vertex;


/* Struct of the entire Graph */
typedef struct
{
    int list_size;
    Energy limit;
    Energy energy;
    Vertex *vertexes;
} Graph;


/* Public Functions */
void graph_resize(PPMImage *, int, int);
void init_graph(Graph *, PPMImage *);
#endif
