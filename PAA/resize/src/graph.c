/*
 * graph.c
 *
 * @author: Gustavo Pantuza Coelho Pinto
 * @since: 17.05.2013
 *
 */
#include <limits.h>
#include <stdlib.h>

#include "priority.h"
#include "debug.h"
#include "color.h"

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
    Vertex *vertexes;
} Graph;


#define NONE  -1
#define SKIP  -2
#define BREAK -3

/**
 * Convert image(x, y) to position to vet[i]
 */
#define XY2POS(img,x,y)  ((img->height * (x)) + (y))

/**
 * Convert position to vet[i] to image(x, y)
 */
#define POS2X(i,img) ((i) / (img)->height)
#define POS2Y(i,img) ((i) % (img)->height)


/**
 * Adjacency control
 */
#define UP_LEFT(v,img)    ((v) - (img->height) - 1)
#define UP_MIDDLE(v,img)  ((v) - 1)
#define UP_RIGHT(v,img)   ((v) + (img->height) - 1)
#define LEFT   0
#define MIDDLE 1
#define RIGHT  2
#define END    3


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
        v = XY2POS(image,x,y);
        graph->vertexes[v].pixel = &(image->pixels[0][y]);
        graph->vertexes[v].adj[LEFT]   = SKIP;
        graph->vertexes[v].adj[MIDDLE] = XY2POS(image,x,y+1);
        graph->vertexes[v].adj[RIGHT]  = XY2POS(image,x+1,y+1);
        graph->vertexes[v].adj[END]    = NONE;
    }

    /* Build the middle vertices */
    for(y = 0; y < image->height - 1; y++)
        for(x = 1; x < image->width - 1; x++)
        {
            v = XY2POS(image,x,y);
            graph->vertexes[v].pixel = &(image->pixels[x][y]);
            graph->vertexes[v].adj[LEFT]   = XY2POS(image,x-1,y+1);
            graph->vertexes[v].adj[MIDDLE] = XY2POS(image,x,y+1);
            graph->vertexes[v].adj[RIGHT]  = XY2POS(image,x+1,y+1);
            graph->vertexes[v].adj[END]    = NONE;
        }

    /* Build the leftmost vertices */
    x = image->width - 1;
    if(x > 0)
        for(y = 0; y < image->height - 1; y++)
        {
            v = XY2POS(image,x,y);
            graph->vertexes[v].pixel = &(image->pixels[x][y]);
            graph->vertexes[v].adj[LEFT]   = XY2POS(image,x-1,y+1);
            graph->vertexes[v].adj[MIDDLE] = XY2POS(image,x,y+1);
            graph->vertexes[v].adj[RIGHT]  = NONE;
            graph->vertexes[v].adj[END]    = NONE;
        }

    /* Build the bottom vertices */
    y = image->height - 1;
    for(x = 0; x < image->width; x++)
    {
        v = XY2POS(image,x,y);
        graph->vertexes[v].pixel = &(image->pixels[x][y]);
        graph->vertexes[v].adj[LEFT]   = NONE;
        graph->vertexes[v].adj[MIDDLE] = NONE;
        graph->vertexes[v].adj[RIGHT]  = NONE;
        graph->vertexes[v].adj[END]    = NONE;
    }
}

/**
 * Free the graph memory
 */
void free_graph(Graph *graph)
{
    free(graph->vertexes);
}

/**
 * Shortest Path solution implemented using the Dijkstra algorithm
 */
int dijkstra(Graph *graph, int source, pri_queue priq,
              Energy *distance, int previous[], Energy shortest_distance)
{
    int vertex, adjacent, v;
    Energy alt;
    int *visited = calloc(graph->list_size, sizeof(int));

    for(v = 0; v < graph->list_size; distance[v++] = ENERGY_MAX);

    previous[source] = NONE;
    distance[source] = graph->vertexes[source].pixel->energy;
    visited[source] = 1;

    priq_purge(priq);
    priq_push(priq, source, distance[source]);

    int dest = NONE;
    while(priq_size(priq))
    {
        vertex = priq_pop(priq);
        v = 0;
        adjacent = graph->vertexes[vertex].adj[v++];

        if(adjacent == NONE)
        {
            dest = vertex;
            break;
        }
#ifdef OPT_GRAPH_SHORTEST_PATH_BREAK
        else
            if (distance[vertex] >= shortest_distance)
            {
                dest = BREAK;
                break;
            }
#endif

        do
        {
            if(adjacent >= 0 && !visited[adjacent])
            {
                alt = distance[vertex] +
                        graph->vertexes[adjacent].pixel->energy;
                if(alt < distance[adjacent])
                {
                    distance[adjacent] = alt;
                    previous[adjacent] = vertex;
                    priq_push(priq, adjacent, alt);
                }
            }
            adjacent = graph->vertexes[vertex].adj[v++];
        } while(adjacent != NONE);
        visited[vertex] = 1;
    }
    free(visited);
    return dest;
}

/**
 * Resize method that calls Dijkstra algorithm for each Vertex inside
 * the Vertexes set of the Graph
 */
void graph_shortest_path(PPMImage *image, int *path)
{
    Graph graph;
    init_graph(&graph, image);
    pri_queue_t priq_s;
    priq_init(&priq_s, graph.list_size);

    int i, x, y, dest;
    Energy *distance = calloc(graph.list_size, sizeof(Energy));
    int *previous = calloc(graph.list_size, sizeof(int));
    Energy shortest_distance;
    shortest_distance = ENERGY_MAX;

    for(x = 0; x < image->width; x++)
    {
        i = XY2POS(image,x,0);

        dest = dijkstra(&graph, i, &priq_s,
                distance, previous, shortest_distance);

#ifdef OPT_GRAPH_SHORTEST_PATH_BREAK
        if(dest == BREAK)
            continue;
        else
#endif
            if(dest < 0)
            {
                fprintf(stderr, "There is no path\n");
                exit(EXIT_FAILURE);
            }

        if (distance[dest] < shortest_distance)
        {
            shortest_distance = distance[dest];
            y = image->height - 1;
            while(dest != i)
            {
                path[y--] = POS2X(dest, image);
                dest = previous[dest];
            }
            path[y] = POS2X(i, image);
            // assert path length
            ASSERT_TRUE(y == 0,
                    fprintf(stderr,
                            "ASSERT: Path length error (must to be %d): %d\n",
                            image->height, y)
            );
        }
    }

    priq_free(&priq_s);
    free_graph(&graph);
    free(distance);
    free(previous);
}

