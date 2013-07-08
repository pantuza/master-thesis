#include <stdlib.h>
#include <vector>
#include <math.h>
#include "graph.h"
#include "color.h"
#include "debug.h"

namespace Graph {

    Vertex::Vertex()
    {
        id = 0;
        c = 0;
        data = NULL;

    }

    void Vertex::_addAdjacent(Vertex *vi, Edge *ei)
    {
        //a.insert(v, v);
        v.push_back(vi);
        e.push_back(ei);
    }

    void Vertex::_delAdjacent(Vertex *vi, Edge *ei)
    {
        //a.erase(a.find(v));
    }

    int Vertex::hasEdgeColor(int ci)
    {
        for (Edge *ei: e)
            if(ei->c == ci)
                return 1;
        return 0;
    }

    Edge::Edge(Vertex *v1e, Vertex *v2e)
    {
        v1 = v1e;
        v2 = v2e;
        c = 0;
        data = NULL;
    }

    Graph::Graph()
    {
        d = 0;
        data = NULL;
        c = 0;
        chromaticNumber = 0;
        chromaticIndex = 0;
        directed = false;
    }

    Graph::~Graph()
    {
        for (Vertex* vi: v)
            delete vi;
        for (Edge* ei: e)
            delete ei;
        v.clear();
        e.clear();
    }

    void Graph::load(FILE *f)
    {
        v.clear();
        e.clear();
        int nv, ne;
        int v1, v2;
        fscanf(f, "%d %d", &nv, &ne);
        std::vector<Vertex *> vertex;
        for (int i = 0; i < nv; i++)
        {
            vertex.push_back(addVertex());
            //v.push_back(vertex[i]);
        }
        for (int i = 0; i < ne; i++)
        {
            fscanf(f, "%d %d", &v1, &v2);
            //e.push_back(new Edge(vertex[v1], vertex[v2]));
            if (directed)
                addEdge(vertex[v1], vertex[v2]);
            else
                addEdgeUnique(vertex[v1], vertex[v2]);
        }
    }

    void Graph::load_colorized(FILE *f)
    {
        v.clear();
        e.clear();
        int nv, ne;
        int v1, v2, c;
        fscanf(f, "%d %d", &nv, &ne);
        std::vector<Vertex *> vertex;
        for (int i = 0; i < nv; i++)
        {
            vertex.push_back(addVertex());
            //v.push_back(vertex[i]);
        }
        for (int i = 0; i < ne; i++)
        {
            fscanf(f, "%d %d %d", &v1, &v2, &c);
            //e.push_back(new Edge(vertex[v1], vertex[v2]));
            addEdge(vertex[v1], vertex[v2])->c = c;
        }
    }

    void Graph::resetId()
    {
        int vc = 0;
        for (Vertex *vi: v)
            vi->id = vc++;
    }

    void Graph::save2dot(FILE *f)
    {
        //int showEdgeLabel = !color.isBasic(chromaticIndex);
        int showEdgeLabel = 1;
        fprintf(f, "digraph g {\n");
        fprintf(f, "node[shape=circle];\n");
        fprintf(f, "edge[arrowsize=0.0, arrowhead=none,  penwidth=3];\n");
        resetId();
        for (Vertex *vi: v)
        {
            fprintf(f, "%d [label=\"v%d\", color=\"%s\"];\n",
                    vi->id, vi->id, color.get(chromaticNumber, vi->c));
        }
        for (Edge *ei: e)
            if (showEdgeLabel)
                fprintf(f, "%d -> %d [label=\"c%d\"color=\"%s\"];\n",
                    ei->v1->id, ei->v2->id, ei->c,
                    color.get(chromaticIndex, ei->c));
            else
                fprintf(f, "%d -> %d [color=\"%s\"];\n",
                    ei->v1->id, ei->v2->id,
                    color.get(chromaticIndex, ei->c));
        fprintf(f, "}\n");

    }

    void Graph::save(FILE *f)
    {
        fprintf(f, "%lu %lu\n", (long unsigned)v.size(), (long unsigned)e.size());
        //fprintf(f, "[%d]\n", d);
        resetId();
        //for (Vertex *vi: v)
        //{
        //    fprintf(f, "%d\n", vi->c);
            //fprintf(f, "(%d)[", vi->getDegree());
            //for (Vertex *va: vi->a)
            //    fprintf(f, "%d ", va->id);
            //fprintf(f, "]\n");
        //}
        for (Edge *ei: e)
            fprintf(f, "%d %d %d\n", ei->v1->id, ei->v2->id, ei->c);
    }

    void Graph::saveRaw(FILE *f)
    {
        fprintf(f, "%lu %lu\n", (long unsigned)v.size(), (long unsigned)e.size());
        resetId();
        for (Edge *ei: e)
            fprintf(f, "%d %d\n", ei->v1->id, ei->v2->id);
    }

    Vertex* Graph::addVertex()
    {
        Vertex *vi = new Vertex();
        v.push_back(vi);
        return vi;
    }

    Edge* Graph::addEdge(Vertex *v1, Vertex *v2)
    {
        Edge *ei = new Edge(v1, v2);
        e.push_back(ei);
        v1->_addAdjacent(v2, ei);
        v2->_addAdjacent(v1, ei);
        // cache degree
        if (d >= 0 && v1->getDegree() > d)
            d = v1->getDegree();
        if (d >= 0 && v2->getDegree() > d)
            d = v2->getDegree();
        return ei;
    }

    Edge* Graph::addEdgeUnique(Vertex *v1, Vertex *v2)
    {
        for (Edge *ei: e)
            if ((ei->v1==v1 && ei->v2==v2) || (ei->v1==v2 && ei->v2==v1))
                return NULL;
        return addEdge(v1, v2);
    }

    int Graph::colorize_edge_woc(const int maxColor)
    {
        int nextColor;
        int numEdges = e.size();
        int countColors = 0;
        DEBUG(fprintf(stderr, "-- Max colors = %d\n", maxColor));
        for (Vertex *vi: v)
        {
            nextColor = 1;
            for (Edge *ei: vi->e)
                if(!ei->c)
                {
                    while(nextColor <= numEdges)
                    {
                        if ((ei->v1->hasEdgeColor(nextColor) ||
                             ei->v2->hasEdgeColor(nextColor)))
                            nextColor++;
                        else
                        {
                            ei->c = nextColor;
                            if (nextColor > countColors)
                                countColors = nextColor;
                            break;
                        }
                    }
                    ASSERT_TRUE(nextColor <= numEdges,
                        fprintf(stderr, "ERROR! Number of colors exceeds number of edges.")
                    );
                }
        }
        return countColors;
    }

    /**
     * Get next edge-colors combination (arrangement) with c colors
     * Complexity (worst case): e
     * Approximately: O(n)
     */
    int Graph::next_combination(const int maxColor)
    {
        for (Edge *ei: e)
            if (ei->c < maxColor)
            {
                ei->c++;
                return 1;
            }
            else
                ei->c = 1;
        return 0;
    }

    /**
     * Verifiy the edge-colorization
     * Complexity (worst case): v * (c + e)
     * Approximately: O(n^2)
     */
    int Graph::valid_colorization(const int maxColor, const int k)
    {
        int colorIndex = maxColor + 1;
        // keep color usage counter
        int colorUse[colorIndex];
        for (Vertex *vi: v)
        {
            // reset color usage counter
            for (int i = 0; i < colorIndex; i++)
                colorUse[i] = 0;
            // verify color usage of all edges connected to current vertex
            for (Edge *ei: vi->e)
                if (colorUse[ei->c] < k)
                    colorUse[ei->c]++;
                else
                    return 0;
        }
        return !colorUse[0];
    }

    /**
     * Set all edge-colors
     * Complexity (worst case): e
     * Approximately: O(n)
     */
    void Graph::set_edge_colors(int c)
    {
        for (Edge *ei: e)
            ei->c = c;
    }

    /**
     * Brute force edge-colorization with minimum number of color
     * Precondition: Graph must be a multigraph
     * Complexity (worst case): sum_(c=1)^(d+1)[e + (c^e * (v * (c + e) + e))]
     * Approximately: O(n^n)
     */
    int Graph::edge_colorize_bf(int k)
    {
        if(k != 1) {
            fprintf(stderr, "Ignoring k, because is't implemented...");
            k = 1;
        }
        // minimum colors is graph degree
        int ci = getDegree();
        // number of edge color used (chromatic index) is limited to
        // graph degree + 1 (Vizing’s Theorem)
        int maxColor = ci + 1;
        // verify all combinations of each chromatic index
        DEBUG(fprintf(stderr, "## Brute force (from %d to %d colors)...\n", ci, maxColor));
        while (ci <= maxColor)
        {
            DEBUG(fprintf(stderr, "-- Colors = %d\n", ci));
            // initial color combination
            // remember: zero means uncolored or "black" edge
            set_edge_colors(1);
            // verify all color combinations
            do
            {
                if (valid_colorization(maxColor, k))
                    return ci;
            } while (next_combination(ci));
            ci++;
        }
        // there's no valid combination
        return 0;
    }

    int Graph::has_valid_edge_colorization(int k)
    {
        return 0;
        //return valid_colorization(getDegree() + 2, k);
    }

    int Graph::edge_colorize_greedy(int k)
    {
        if(k != 1) {
            fprintf(stderr, "Ignoring k, because is't implemented...");
            k = 1;
        }
        int colorizedEdges = colorize_edge_woc(k);
        ASSERT_TRUE(valid_colorization(e.size(), k),
            fprintf(stderr, "ERROR! Greed colorization failure...");
        );
        return colorizedEdges;
    }

    void Graph::complete(int nv)
    {
        for (Vertex* vi: v)
            delete vi;
        for (Edge* ei: e)
            delete ei;
        std::vector<Vertex *> vertex;
        for(int i=0; i < nv; i++)
            vertex.push_back(addVertex());
        for (int i=0; i < (nv - 1); i++)
            for (int j=i+1; j < nv; j++)
                addEdge(vertex[i], vertex[j]);
    }

}
