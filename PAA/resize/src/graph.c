#include <limits.h>

#include "graph.h"

#define VISITED INT_MAX - 1


/**
 * Finds which position inside the vertexes list on vertice will be placed 
 */
int get_adj_vertex(int v, int width, int height, int diff)
{
    int edge_pos = v + width + diff;
    int max_pos = width * height;

    return edge_pos < max_pos ? edge_pos : max_pos;
}


/**
 * Builds an Edge considering the current positions (i, j) inside the image 
 */
Edge* get_edge(PPMImage *image, int i, int j, int v, int diff)
{
    Edge *edge = malloc(sizeof(Edge));
    edge->vertex = get_adj_vertex(v, image->width, image->height, diff);
    edge->energy = image->pixels[j][i].energy;

    return edge;
}


/**
 * Constructs a rightmost vertex. Do not build vertexes beyond the image
 */
Vertex get_right_vertex(PPMImage *image, int i, int j, int v)
{
    Vertex vertex;
    vertex.energy = image->pixels[j][i].energy;

    if(i+1 <= image->height)
    {
        vertex.edge = get_edge(image, i+1, j-1, v, -1);
        vertex.edge->next = get_edge(image, i+1, j, v, 0);
        ((Edge *)(vertex.edge->next))->next = NULL;
    
    } else vertex.edge = NULL;

    return vertex;
}


/**
 * Construcs a middle vertex with all its edges
 */
Vertex get_middle_vertex(PPMImage *image, int i, int j, int v)
{

    Vertex vertex;
    vertex.energy = image->pixels[j][i].energy;
    
    if(i+1 <= image->height)
    {
        vertex.edge = get_edge(image, i+1, j-1, v, -1);
        vertex.edge->next = get_edge(image, i+1, j, v, 0);
        ((Edge *)vertex.edge->next)->next = get_edge(image, i+1, j+1, v, 1);
        ((Edge *)((Edge *)vertex.edge->next)->next)->next = NULL;
    
    } else vertex.edge = NULL;

    return vertex;
}


/**
 * Constructs a leftmost vertex. Do not build vertexes beyond the image
 */
Vertex get_left_vertex(PPMImage *image, int i, int j, int v)
{
    Vertex vertex;
    vertex.energy = image->pixels[j][i].energy;

    if(i+1 <= image->height)
    {
        vertex.edge = get_edge(image, i+1, j, v, 0);
        vertex.edge->next = get_edge(image, i+1, j+1, v, 1);
        ((Edge *)vertex.edge->next)->next = NULL;
    
    } else vertex.edge = NULL;

    return vertex;
}


/**
 * Creates a graph based on the image pixels and fill the values for 
 * its vertexes and edges
 */
void init_graph(Graph *graph, PPMImage *image)
{
    graph->list_size = image->width * image->height;
    graph->vertexes = (Vertex *) malloc(graph->list_size * sizeof(Vertex));

    int v = 0;
    /* Build the leftmost vertices */
    for(int i=0; i < image->height; i++)
        graph->vertexes[v++] = get_left_vertex(image, i, 0, v);
      
    /* Build the middle vertices */
    for(int i=1; i < image->height; i++)
        for(int j=0; j < image->width - 2; j++)
            graph->vertexes[v++] = get_middle_vertex(image, i, j, v);

    /* Build the rightmost vertices */
    for(int i=0; i < image->height; i++)
        graph->vertexes[v++] = get_right_vertex(image, i, image->width-1, v);
}


int remove_smallest(int size, double distance[])
{
    double min = distance[0];
    int ind = 0;

    for(int i = 1; i < size; i++)
        if(distance[i] < min)
        {
            min = distance[i];
            ind = i;
        }
    distance[ind] = VISITED;
    return ind;
}




/**
 * Shortest Path solution implemented using the Dijkstra algorithm
 */
int dijkstra(Graph *graph, int source, 
              int previous[], double distance[])
{
    distance[source] = 0;
    int exists = 1;
    int smallest;

    for(int i = 0; i < graph->list_size; previous[i++] = source);

    while(exists)
    {
        smallest = remove_smallest(graph->list_size, distance);
        exists--;

        printf("exists: %d, smallest: %d\t", exists, smallest);
        Edge *adjacent = graph->vertexes[smallest].edge;

        if(adjacent->next == NULL) return smallest;
        while(adjacent->next != NULL)
        {
            printf("dist[%d]: %lf\n", adjacent->vertex, distance[adjacent->vertex]);
            if(distance[adjacent->vertex] == INT_MAX)
                exists++;
            
            if(distance[adjacent->vertex] != VISITED)
            {

                int alt = distance[smallest] + adjacent->energy;

                if(alt < distance[adjacent->vertex])
                {
                    distance[adjacent->vertex] = alt;
                    previous[adjacent->vertex] = smallest;
                }
            }
            printf("next:%d\n", ((Edge *)adjacent->next)->vertex);
            adjacent = adjacent->next;
        }
    }
    return -1;
}


/**
 * Resize method that calls Dijkstra algorithm for each Vertex inside 
 * the Vertexes set of the Graph
 */
void graph_resize(PPMImage *image, int width, int height)
{
    
    Graph graph;
    init_graph(&graph, image);
    
    int previous[graph.list_size];
    double distance[graph.list_size];

    for(int i = 0; i < graph.list_size; distance[i++] = INT_MAX);

    do
    {
        for(int i = 0; i < graph.list_size; i += image->height)
        {
            printf("i: %d\n", i);
            int dest = dijkstra(&graph, i, previous, distance);

            if(dest == -1)
            {
                fprintf(stderr, "There is no path\n");
                exit(EXIT_FAILURE);
            }
            
            while(dest != i)
            {
                printf("%d,", dest);
                dest = previous[dest];
            }
        }

    } while(width && height);

}
