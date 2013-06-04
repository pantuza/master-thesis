#include "graph.h"


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
Edge get_edge(PPMImage *image, int i, int j, int v, int diff)
{
    Edge edge;
    edge.vertex = get_adj_vertex(v, image->width, image->height, diff);
    edge.energy = image->pixels[i][j].energy;

    return edge;
}


/**
 * Constructs a rightmost vertex. Do not build vertexes beyond the image
 */
Vertex get_right_vertex(PPMImage *image, int i, int j, int v)
{
    Vertex vertex;
    vertex.energy = image->pixels[i][j].energy;

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
    vertex.energy = image->pixels[i][j].energy;
    
    if(i+1 <= image->height)
    {
        vertex.edge = get_edge(image, i+1, j-1, v, -1);
        vertex.edge.next = get_edge(image, i+1, j, v, 0);
        vertex.edge.next.next = get_edge(image, i+1, j+1, v, 1);
        vertex.edge.next.next.next = NULL;
    
    } else vertex.edge = NULL;

    return vertex;
}


/**
 * Constructs a leftmost vertex. Do not build vertexes beyond the image
 */
Vertex get_left_vertex(PPMImage *image, int i, int j, int v)
{
    Vertex vertex;
    vertex.energy = image->pixels[i][j].energy;

    if(i+1 <= image->height)
    {
        vertex.edge = get_edge(image, i+1, j, v, 0);
        vertex.edge.next = get_edge(image, i+1, j+1, v, 1);
        vertex.edge.next.next = NULL;
    
    } else vertex.edge = NULL;

    return vertex;
}


/**
 * Creates a graph based on the image pixels and fill the values for 
 * its vertexes and edges
 */
Graph* init_graph(PPMImage *image)
{
    Graph graph;
    graph.list_size = image->width * image->height;
    graph.vertexes = (Vertex *) malloc(graph.list_size * sizeof(Vertex *));

    int v = 0;
    while(v < graph.list_size)
    {
        /* Build the leftmost vertices */
        for(int i=0; i < image->height-1; i++)
            graph.vertexes[v++] = get_left_vertex(image, i, 0, v);
       
        /* Build the middle vertices */
        for(int j=1; j < image->height; j++)
            for(int i=0; i < image->width - 2; i++)
                graph.vertexes[v++] = get_middle_vertex(image, i, j, v);

        /* Build the rightmost vertices */
        for(int i=0; i < image->height-1; i++)
            graph.vertexes[v++] = get_right_vertex(image, i, 
                                                   image->width-1, v);
    }

    return &graph;
}


/**
 * Shortest Path solution implemented using the Dijkstra algorithm
 */
void dijkstra(Graph *graph, Vertex *source, 
              Vertex **previous, Vertex **distance)
{
    // here comes the sun
}


/**
 * Resize method that calls Dijkstra algorithm for each Vertex inside 
 * the Vertexes set of the Graph
 */
void graph_resize(PPMImage *image, int width, int height)
{
    
    Graph graph = init_graph(image);
    
    Vertex previous[graph.list_size];
    Vertex distance[graph.list_size];

    do
    {
        for(int i=0; i < graph.list_size; i++)
            dijkstra(graph, graph.vertexes[i], previous, distance);

    } while(width && height);

}
