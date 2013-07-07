#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <list>
#include <map>
#include "color.h"

namespace Graph {

    class Edge;
    typedef std::list<Edge *> Edges;
    class Vertex;
    typedef std::list<Vertex *> Vertices;
    class Graph;

    class Vertex
    {
        private:
            int id;
            int c;
            Vertices v;
            Edges e;
            void *data;
            void _addAdjacent(Vertex *, Edge *);
            void _delAdjacent(Vertex *, Edge *);
        public:
            friend class Edge;
            friend class Graph;
            Vertex();
            inline int getColor() { return c; }
            inline int getDegree() { return v.size(); }
            int hasEdgeColor(int ci);
    };

    class Edge
    {
        private:
            Vertex *v1, *v2;
            int c;
            void *data;
        public:
            friend class Vertex;
            friend class Graph;
            Edge(Vertex *, Vertex *);
            inline int getColor() { return c; }
            inline Vertex* getVertex1() { return v1; }
            inline Vertex* getVertex2() { return v2; }
    };

    class Graph
    {
        private:
            Vertices v;
            //
            Edges e;
            //
            int d;
            //
            void *data;
            //
            int c;
            //
            int chromaticNumber;
            //
            int chromaticIndex;
            //
            Color color;
            //
            void resetId();
            int colorize_edge_woc(const int);
            int next_combination(const int);
            int valid_colorization(const int, const int);
        public:
            friend class Vertex;
            friend class Edge;
            /* Public Functions */
            Graph();
            ~Graph();
            void load(FILE *);
            void load_colorized(FILE *);
            void save2dot(FILE *);
            void save(FILE *);
            Graph line();
            void set_edge_colors(int);
            int edge_colorize_bf(int);
            int edge_colorize_greedy(int);
            int has_valid_edge_colorization(int);
            int vertex_colorize();
            inline int numVertex() { return v.size(); };
            inline int numEdge() { return e.size(); };
            inline int getDegree() { return d; };
            inline int getColor() { return c; };
            inline int getCindex() { return chromaticIndex; };
            inline int getCnumber() { return chromaticNumber; };
            Vertex* addVertex();
            Edge* addEdge(Vertex*, Vertex*);
    };

}
#endif
