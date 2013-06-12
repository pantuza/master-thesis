#include <limits.h>

#include "graph.h"

#define MAX_DISTANCE INT_MAX
#define VISITED (MAX_DISTANCE - 1)
#define NONE -1

/**
 * Convert image(x, y) to position to vet[i]
 */
#define POS_XY(img,x,y)  ((img->height * (x)) + (y))


/**
 * Creates a graph based on the image pixels and fill the values for
 * its vertexes and edges
 */
void init_graph(Graph *graph, PPMImage *image)
{
    graph->list_size = image->width * image->height;
    graph->vertexes = (Vertex *) calloc(graph->list_size, sizeof(Vertex));

    int v, x, y;
    /* Build the leftmost vertices */
    x = 0;
    for(y = 0; y < image->height - 1; y++)
    {
        v = POS_XY(image,x,y);
        //printf("|-POS(%d,%d): %d\n",x,y,v);
        //printf("----adj: %d %d\n", POS_XY(image,x,y+1), POS_XY(image,x+1,y+1));
        graph->vertexes[v].pixel = &(image->pixels[y][0]);
        graph->vertexes[v].adj[0] = POS_XY(image,x,y+1);
        graph->vertexes[v].adj[1] = POS_XY(image,x+1,y+1);
        graph->vertexes[v].adj[2] = NONE;
        graph->vertexes[v].adj[3] = NONE;
    }

    /* Build the middle vertices */
    for(y = 0; y < image->height - 1; y++)
        for(int x = 1; x < image->width - 1; x++)
        {
            v = POS_XY(image,x,y);
            //printf("--POS(%d,%d): %d\n",x,y,v);
            //printf("----adj: %d %d %d\n",POS_XY(image,x-1,y+1), POS_XY(image,x,y+1), POS_XY(image,x+1,y+1));
            graph->vertexes[v].pixel = &(image->pixels[y][x]);
            graph->vertexes[v].adj[0] = POS_XY(image,x-1,y+1);
            graph->vertexes[v].adj[1] = POS_XY(image,x,y+1);
            graph->vertexes[v].adj[2] = POS_XY(image,x+1,y+1);
            graph->vertexes[v].adj[3] = NONE;
        }

    /* Build the leftmost vertices */
    x = image->width - 1;
    for(y = 0; y < image->height - 1; y++)
    {
        v = POS_XY(image,x,y);
        //printf("-|POS(%d,%d): %d\n",x,y,v);
        //printf("----adj: %d %d\n",POS_XY(image,x-1,y+1), POS_XY(image,x,y+1));
        graph->vertexes[v].pixel = &(image->pixels[y][x]);
        graph->vertexes[v].adj[0] = POS_XY(image,x-1,y+1);
        graph->vertexes[v].adj[1] = POS_XY(image,x,y+1);
        graph->vertexes[v].adj[2] = NONE;
        graph->vertexes[v].adj[3] = NONE;
    }

    /* Build the bottom vertices */
    y = image->height - 1;
    for(x = 0; x < image->width; x++)
    {
        v = POS_XY(image,x,y);
        //printf("__POS(%d,%d): %d\n",x,y,v);
        //printf("----adj: NONE\n");
        graph->vertexes[v].pixel = &(image->pixels[y][x]);
        graph->vertexes[v].adj[0] = NONE;
        graph->vertexes[v].adj[1] = NONE;
        graph->vertexes[v].adj[2] = NONE;
        graph->vertexes[v].adj[3] = NONE;
    }
}

/**
 * Free the graph memory
 */
void free_graph(Graph *graph)
{
    free(graph->vertexes);
}

int get_smallest(int size, double distance[])
{
    double min = distance[0];
    int ind = 0;

    for(int i = 1; i < size; i++)
        if(distance[i] < min)
        {
            min = distance[i];
            ind = i;
        }
    return ind;
}


/**
 * Shortest Path solution implemented using the Dijkstra algorithm
 */
int dijkstra(Graph *graph, int source,
              double distance[], int previous[])
{
    int vertex, adjacent, v;
    double alt;

    for(int i = 0; i < graph->list_size; distance[i++] = MAX_DISTANCE);

    distance[source] = graph->vertexes[source].pixel->energy;
    previous[source] = NONE;
    int exists = 1;

    //printf("source: %d, distance: %f\n", source, distance[source]);

    while(exists)
    {
        vertex = get_smallest(graph->list_size, distance);
        //visited[vertex] = 1;
        exists--;

        //printf("exists: %d, vertex: %d, distance: %f, energy:%f \n",
        //        exists, vertex,distance[vertex],graph->vertexes[vertex].pixel->energy);
        v = 0;
        adjacent = graph->vertexes[vertex].adj[v++];
        //printf(" -- adj[%d]:%d, distance: %f\n", v, adjacent, distance[adjacent]);

        if(adjacent == NONE) return vertex;

        while(adjacent != NONE)
        {
            if(distance[adjacent] != VISITED) {
                exists++;
                //printf(" ++ dist[%d]: %f, prev=%d\n", adjacent, distance[adjacent], previous[adjacent]);
            }
            //else
                //printf(" -- dist[%d]: %f, prev=%d\n", adjacent, distance[adjacent], previous[adjacent]);

            alt = distance[vertex] + graph->vertexes[adjacent].pixel->energy;
            //printf(" -- alt: %f\n", alt);
            if(alt < distance[adjacent])
            {
                distance[adjacent] = alt;
                previous[adjacent] = vertex;
            }
            adjacent = graph->vertexes[vertex].adj[v++];
            //printf(" -- adj[%d]:%d\n", v, adjacent);
        }
        distance[vertex] = VISITED;
    }
    return NONE;
}


/**
 * Resize method that calls Dijkstra algorithm for each Vertex inside
 * the Vertexes set of the Graph
 */
void graph_resize(PPMImage *image, int width, int height)
{

/*
    for(int y = 0; y < image->height; y++) {
        for(int x = 0; x < image->width; x++)
            printf("%f ", image->pixels[y][x].energy);
        printf("\n");
    }
*/

    Graph graph;
    init_graph(&graph, image);

    int i, x, y;
    double *distance = calloc(graph.list_size, sizeof(double));
    int *previous = calloc(graph.list_size, sizeof(int));
    int *path = calloc(image->height, sizeof(int));
    double minimum;
    double max_energy = image->energy * image->height;

    if (width > image->width)
        width = image->width;

    do {
        minimum = MAX_DISTANCE;
        for(x = 0; x < image->width; x++)
        {
            i = POS_XY(image,x,0);
            //printf("w: %d, i: %d\n", width, i);
            int dest = dijkstra(&graph, i, distance, previous);

            if(dest == NONE)
            {
                fprintf(stderr, "There is no path\n");
                exit(EXIT_FAILURE);
            }

            //printf("dest: %d, distance: %f\n", dest, distance[dest]);
            if (distance[dest] < minimum)
            {
                minimum = distance[dest];
                //fprintf(stderr, "Minimum distance = %f\n", minimum);
                y = 0;
                while(dest != i)
                {
                    //printf("%d,", dest);
                    path[y++] = dest;
                    dest = previous[dest];
                }
                path[y++] = dest;
                //printf("%d\n", dest);
            }
        }

        for(y = 0; y < image->height; y++)
        {
            graph.vertexes[path[y]].pixel->R = 255;
            graph.vertexes[path[y]].pixel->G = 0;
            graph.vertexes[path[y]].pixel->B = 0;
            graph.vertexes[path[y]].pixel->energy = max_energy;
        }
    } while(--width);

    free_graph(&graph);
    free(distance);
    free(previous);
    free(path);
}
