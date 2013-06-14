#include <limits.h>

#include "graph.h"
#include "priority.h"
#include "debug.h"
#include "color.h"

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
//#define POS2XY(i,img,x,y) {x = i / img->height; y = i % img->height;}


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
    graph->limit = image->energy * graph->list_size;
    graph->energy = image->energy * image->height;
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

#ifdef OPT_GRAPH_REMOVE_SHORTEST_PATH
/**
 * Remove a vertices from graph and adjust its adjacency
 */
inline static void remove_vertex(Graph *graph, PPMImage *image,
        int vertex, int previous)
{
    int left   = UP_LEFT(vertex, image);
    int middle = UP_MIDDLE(vertex, image);
    int right  = UP_RIGHT(vertex, image);

    if (previous != left && left >= 0)
        graph->vertexes[left].adj[RIGHT] = graph->vertexes[middle].adj[RIGHT];

    if (previous != right && right < graph->list_size)
        graph->vertexes[right].adj[LEFT] = graph->vertexes[middle].adj[LEFT];

    if (previous != middle)
    {
        if (previous == left)
            graph->vertexes[middle].adj[MIDDLE] =
                    graph->vertexes[left].adj[MIDDLE];
        else //if (previous == right)
            graph->vertexes[middle].adj[MIDDLE] =
                    graph->vertexes[right].adj[MIDDLE];
    }
}
#endif

/**
 * Shortest Path solution implemented using the Dijkstra algorithm
 */
int dijkstra(Graph *graph, int source,
              Energy *distance, int previous[], Energy shortest_distance)
{
    int vertex, adjacent, v;
    Energy alt;
    Energy visited     = graph->limit + 4;
    Energy no_visited  = visited - 2;

    for(v = 0; v < graph->list_size; distance[v++] = no_visited);
    previous[source] = NONE;
    distance[source] = 0;

    pri_queue_t priq_s;
    pri_queue priq = &priq_s;
    priq_init(priq, graph->list_size);
    priq_push(priq, source, graph->vertexes[source].pixel->energy);

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
            if(adjacent != SKIP && distance[adjacent] <= visited)
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
        distance[vertex] = visited;
    }
    priq_free(priq);
    return dest;
}

/**
 * Resize method that calls Dijkstra algorithm for each Vertex inside
 * the Vertexes set of the Graph
 */
void graph_resize(PPMImage *image, int width, int height)
{

    if (width > image->width)
    {
        fprintf(stderr, "Image size too small to resize.");
        exit(EXIT_FAILURE);
    }

    Graph graph;
    init_graph(&graph, image);

    int i, x, y, dest;
    Energy *distance = calloc(graph.list_size, sizeof(Energy));
    int *previous = calloc(graph.list_size, sizeof(int));
    int *path = calloc(image->height, sizeof(int));
    Energy shortest_distance;

    Color color;
    init_color(&color, width);

    while(width--)
    {
        shortest_distance = graph.limit + 1;
        for(x = 0; x < image->width; x++)
        {
            i = XY2POS(image,x,0);

#ifdef OPT_GRAPH_IGNORE_CARVED_POINT
            if (graph.vertexes[i].pixel->energy >= graph.energy)
                continue;
#endif

            dest = dijkstra(&graph, i, distance, previous, shortest_distance);

#ifdef OPT_GRAPH_SHORTEST_PATH_BREAK
            if(dest == BREAK)
                continue;
            else
#endif
                if(dest == NONE)
                {
                    fprintf(stderr, "There is no path\n");
                    exit(EXIT_FAILURE);
                }

            if (distance[dest] < shortest_distance)
            {
                shortest_distance = distance[dest];
                y = 0;
                while(dest != i)
                {
                    path[y++] = dest;
                    dest = previous[dest];
                }
                path[y++] = i;
                // assert path length
                ASSERT_TRUE(y == image->height,
                        fprintf(stderr,
                                "ASSERT: Path length: %d\n (must to be %d)\n",
                                y, image->height)
                );
            }
        }

        for(y = 0; y < image->height; y++)
        {
            graph.vertexes[path[y]].pixel->R = color.r;
            graph.vertexes[path[y]].pixel->G = color.g;
            graph.vertexes[path[y]].pixel->B = color.b;
            graph.vertexes[path[y]].pixel->energy = graph.energy;
        }

#ifdef OPT_IMAGE_SEAM_DIFF_COLORS
        next_color(&color);
#endif

#ifdef OPT_GRAPH_REMOVE_SHORTEST_PATH
        for(y = 1; y < image->height - 1; y++)
            remove_vertex(&graph, image, path[y], path[y-1]);
#endif
    };

    free_graph(&graph);
    free(distance);
    free(previous);
    free(path);
}
