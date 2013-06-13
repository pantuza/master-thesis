#include <limits.h>

#include "graph.h"
#include "priority.h"
#include "debug.h"

#define NONE  -1
#define BREAK -2

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
    graph->limit = image->energy * graph->list_size;
    graph->energy = image->energy * image->height;
    graph->vertexes = (Vertex *) calloc(graph->list_size, sizeof(Vertex));

    int v, x, y;
    /* Build the leftmost vertices */
    x = 0;
    for(y = 0; y < image->height - 1; y++)
    {
        v = POS_XY(image,x,y);
        //fprintf(stderr,"|-POS(%d,%d): %d\n",x,y,v);
        //fprintf(stderr,"----adj: %d %d\n", POS_XY(image,x,y+1), POS_XY(image,x+1,y+1));
        graph->vertexes[v].pixel = &(image->pixels[y][0]);
        graph->vertexes[v].adj[0] = POS_XY(image,x,y+1);
        graph->vertexes[v].adj[1] = POS_XY(image,x+1,y+1);
        graph->vertexes[v].adj[2] = NONE;
        graph->vertexes[v].adj[3] = NONE;
    }

    /* Build the middle vertices */
    for(y = 0; y < image->height - 1; y++)
        for(x = 1; x < image->width - 1; x++)
        {
            v = POS_XY(image,x,y);
            //fprintf(stderr,"--POS(%d,%d): %d\n",x,y,v);
            //fprintf(stderr,"----adj: %d %d %d\n",POS_XY(image,x-1,y+1), POS_XY(image,x,y+1), POS_XY(image,x+1,y+1));
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
        //fprintf(stderr,"-|POS(%d,%d): %d\n",x,y,v);
        //fprintf(stderr,"----adj: %d %d\n",POS_XY(image,x-1,y+1), POS_XY(image,x,y+1));
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
        //fprintf(stderr,"__POS(%d,%d): %d\n",x,y,v);
        //fprintf(stderr,"----adj: NONE\n");
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

/**
 * Shortest Path solution implemented using the Dijkstra algorithm
 */
int dijkstra(Graph *graph, int source,
              Energy *distance, int previous[], Energy shortest_distance)
{
    int vertex, adjacent, v;
    Energy alt;
    Energy visited     = graph->limit;
    Energy no_visited  = visited - 2;
    DEBUG1(fprintf(stderr,"visited: %f, no_visite: %f\n", visited, no_visited));

    for(v = 0; v < graph->list_size; distance[v++] = no_visited);
    previous[source] = NONE;
    distance[source] = 0;

    pri_queue_t priq_s;
    pri_queue priq = &priq_s;

    priq_init(priq, graph->list_size);
    priq_push(priq, source, graph->vertexes[source].pixel->energy);

    DEBUG1(fprintf(stderr,"source: %d, distance: %f\n", source, distance[source]));

    int dest = NONE;
    while(priq_size(priq))
    {
        vertex = priq_pop(priq);

        DEBUG1(fprintf(stderr,"exists: %d, vertex: %d, distance: %f, energy:%f \n",
                priq_size(priq), vertex,distance[vertex],
                graph->vertexes[vertex].pixel->energy));
        v = 0;
        adjacent = graph->vertexes[vertex].adj[v++];
        DEBUG1(fprintf(stderr," -- adj[%d]:%d, distance: %f\n", v, adjacent, distance[adjacent]));

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
            if(distance[adjacent] <= visited)
            {
                alt = distance[vertex] + graph->vertexes[adjacent].pixel->energy;
                DEBUG1(fprintf(stderr," -- alt: %f < %f\n", alt, distance[adjacent]));
                if(alt < distance[adjacent])
                {
                    distance[adjacent] = alt;
                    previous[adjacent] = vertex;
                    priq_push(priq, adjacent, alt);
                }
            }
            adjacent = graph->vertexes[vertex].adj[v++];
            DEBUG1(fprintf(stderr," -- adj[%d]:%d\n", v, adjacent));
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

    DEBUG2(
            for(int y = 0; y < image->height; y++) {
                for(int x = 0; x < image->width; x++)
                    fprintf(stderr,"%f ", image->pixels[y][x].energy);
                fprintf(stderr,"\n");
            }
    );

    Graph graph;
    init_graph(&graph, image);

    int i, x, y, dest;
    Energy *distance = calloc(graph.list_size, sizeof(Energy));
    int *previous = calloc(graph.list_size, sizeof(int));
    int *path = calloc(image->height, sizeof(int));
    Energy shortest_distance;

    if (width > image->width)
    {
        fprintf(stderr, "Image size too small to resize.");
        exit(EXIT_FAILURE);
    }

    while(width--)
    {
        shortest_distance = graph.limit + 1;
        for(x = 0; x < image->width; x++)
        {
            i = POS_XY(image,x,0);
            DEBUG(fprintf(stderr,"w: %d, x: %d/%dx%d, i: %d\n",
                    width, x, image->width, image->height, i));

#ifdef OPT_GRAPH_IGNORE_CARVING
            if (graph.vertexes[i].pixel->energy >= graph.energy)
            {
                DEBUG(fprintf(stderr,
                        "Energy here?: %d = %f\n",
                        i, graph.vertexes[i].pixel->energy)
                )
                continue;
            }
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

            DEBUG(fprintf(stderr,"dest: %d, distance: %f\n", dest, distance[dest]));
            if (distance[dest] < shortest_distance)
            {
                shortest_distance = distance[dest];
                DEBUG(fprintf(stderr, "Minimum distance = %f\n",
                        shortest_distance));
                y = 0;
                while(dest != i)
                {
                    DEBUG(fprintf(stderr,"%d,", dest));
                    path[y++] = dest;
                    dest = previous[dest];
                }
                path[y++] = i;
                // assert path length
                ASSERT_TRUE(y != image->height,
                        fprintf(stderr,
                                "Path length!: %d\n (must to be %d)\n",
                                y, image->height)
                );
                DEBUG(fprintf(stderr,"%d\n", dest));
            }
        }

        for(y = 0; y < image->height; y++)
        {
            graph.vertexes[path[y]].pixel->R = 255;
            graph.vertexes[path[y]].pixel->G = 0;
            graph.vertexes[path[y]].pixel->B = 0;
            graph.vertexes[path[y]].pixel->energy = graph.energy;
        }
    };

    free_graph(&graph);
    free(distance);
    free(previous);
    free(path);
}
