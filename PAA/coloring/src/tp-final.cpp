/**
 * tp-final program
 *
 * @author: Gustavo Pantuza
 * @since: 17.05.2013
 *
 */


#include <time.h>
#include <unistd.h>
#include <unistd.h>
#include <getopt.h>

#include "tp-final.h"
#include "file.h"
#include "graph.h"
#include "baseline.h"
#include "debug.h"

/* Time Function */
float timediff(clock_t end, clock_t start)
{
    return ((float) end - (float) start) / (float) CLOCKS_PER_SEC;
}

void saveDot(const char *name, Graph::Graph *g)
{
    clock_t start;
    clock_t end;
    char filename[1024];
    /* Export graph in DOT*/
    start = clock();
    sprintf(filename, "%s.dot", name);
    FILE *dot = openfile(filename, WRITE_MODE);
    g->save2dot(dot);
    closefile(dot);
    end = clock();
    INFO(fprintf(stderr, "#- Export DOT in %f seconds: %s\n",
            timediff(end, start), filename));

}

void saveColoredGraph(const char *name, Graph::Graph *g)
{
    clock_t start;
    clock_t end;
    char filename[1024];

    /* exports to output file */
    start = clock();
    sprintf(filename, "%s.txt", name);
    FILE *out = openfile(filename, WRITE_MODE);
    g->save(out);
    closefile(out);
    end = clock();
    INFO(fprintf(stderr, "#- Export colored graph in %f seconds: %s\n",
        timediff(end, start), filename));
}

void saveGraph(const char *name, Graph::Graph *g, char filename[FILENAME_MAX])
{
    clock_t start;
    clock_t end;

    /* exports to output file */
    start = clock();
    sprintf(filename, "%s.txt", name);
    FILE *out = openfile(filename, WRITE_MODE);
    g->saveRaw(out);
    closefile(out);
    end = clock();
    INFO(fprintf(stderr, "#- Export raw graph in %f seconds: %s\n",
        timediff(end, start), filename));
}

void save(const char *name, Graph::Graph *g)
{
    saveColoredGraph(name, g);
    saveDot(name, g);
}

#define MIN(a,b) ((a)<(b)?(a):(b))

void analyse(char *graphname, Graph::Graph *g, int cie, float tie,
        int ci1, float ti1, int ci2, float ti2, int cig, float tig)
{
    char filename[FILENAME_MAX];
    sprintf(filename, "analisys.txt");
    FILE *out = openfile(filename, APPEND_MODE);
    int X, G;
    G = g->getDegree();
    if (cie == 0)
        X = MIN(MIN(ci1, ci2),cig);
    else
        X = cie;
    fprintf(out, "%s, %d,%d,%d,%d, %d,%d,%d,%f, %d,%d,%d,%f, %d,%d,%d,%f, %d,%d,%d,%f\n",
            graphname, g->numVertex(), g->numEdge(), G, X,
            cie, 0, 0, tie,
            ci1, ci1 - G, ci1 - X, ti1,
            ci2, ci2 - G, ci2 - X, ti2,
            cig, cig - G, cig - X, tig);
}

int main(int argc, char *argv[])
{
    char *input, *output;
    if(argc < 2) {
        fprintf(stderr, "Usage: %s <input> [<output>] [<analysis>]\n", argv[0]);
        return EXIT_FAILURE;
    } else {
        if (argc < 3)
            output = NULL;
        else
            output = argv[2];
    }

    input = argv[1];
    char filename[FILENAME_MAX];
    FILE *outH1, *outH2;
    char fileH1[FILENAME_MAX];
    char fileH2[FILENAME_MAX];
    if (output != NULL) {
        sprintf(fileH1, "%s_h1", output);
        outH1 = openfile(fileH1, WRITE_MODE);
        sprintf(fileH2, "%s_h2", output);
        outH2 = openfile(fileH2, WRITE_MODE);
    } else {
        outH1 = NULL;
        outH2 = NULL;
    }
    //setvbuf(stdout, NULL, _IOFBF, FILE_BUFFER_SIZE);
    setvbuf(stderr, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);

    // Program execution options
    
    /* Timers */
    clock_t start;
    clock_t end;
    int ci1, ci2, cie, cig;
    float ti1, ti2, tie, tig;
    Graph::Graph g;

//#define COMPLETE_GRAPH
#ifdef COMPLETE_GRAPH
    g.complete(atoi(input));
    FILE *in;
    INFO(
        fprintf(stderr, "## Complete Graph '%s' G(%d, %d) deg(G)=%d\n", input,
            g.numVertex(), g.numEdge(), g.getDegree());
    );
#else
    /* Imports input file */
    start = clock();
    FILE *in = openfile(input, READ_MODE);
    g.load(in);
    closefile(in);
    end = clock();
    INFO(
        fprintf(stderr, "## Graph '%s' G(%d, %d) deg(G)=%d\n", input,
                g.numVertex(), g.numEdge(), g.getDegree());
        fprintf(stderr, "--Import in %f seconds\n", timediff(end, start))
    );
#endif

    saveGraph(input, &g, filename);
    INFO(fprintf(stderr, "-- Exporting undirected graph '%s'\n", filename));

    // Coloring (heuristic 1)
    start = clock();
    FILE *inBase1 = openfile(filename, READ_MODE);
    ci1 = H1::edge_coloring(inBase1, outH1);
    if (outH1 != NULL) closefile(outH1);
    closefile(inBase1);
    end = clock();
    ti1 = timediff(end, start);
    INFO(
        fprintf(stderr, "#Chromatic index (heuristic misraGriesNew) = %d\n", ci1);
        if (ci1 == g.getDegree())
            fprintf(stderr, "-- Class 1\n");
        else
            fprintf(stderr, "-- Class 2\n");
        fprintf(stderr, "- Coloring in %f seconds\n", ti1);
    );

    // Coloring (heuristic 2)
    start = clock();
    FILE *inBase2 = openfile(filename, READ_MODE);
    ci2 = H2::edge_coloring(inBase2, outH2);
    if (outH2 != NULL) closefile(outH2);
    closefile(inBase2);
    end = clock();
    ti2 = timediff(end, start);
    INFO(
        fprintf(stderr, "#Chromatic index (heuristic edgeColor4) = %d\n", ci2);
        if (ci2 == g.getDegree())
            fprintf(stderr, "-- Class 1\n");
        else
            fprintf(stderr, "-- Class 2\n");
        fprintf(stderr, "- Coloring in %f seconds\n", ti2);
    );

#ifdef COMPLETE_GRAPH
    cie = ((g.numVertex() % 2) == 0 ? g.numVertex() - 1: g.numVertex());
    tie = 0.0;
    INFO(
        fprintf(stderr, "#Chromatic index (completed grah) = %d\n", cie);
        if (cie == g.getDegree())
            fprintf(stderr, "-- Class 1\n");
        else
            fprintf(stderr, "-- Class 2\n");
        fprintf(stderr, "- Coloring in %f seconds\n", tie);
    );
#else
    //Coloring (baseline exact)
    int C = MIN(ci1, ci2);
    if (C > g.getDegree() && g.numVertex() <= 10) {
        start = clock();
        g.set_edge_colors(0);
        cie = g.edge_colorize_bf(1);
        end = clock();
        tie = timediff(end, start);
        INFO(
            fprintf(stderr, "#Chromatic index (baseline brute force) = %d\n", cie);
            if (cie == g.getDegree())
                fprintf(stderr, "-- Class 1\n");
            else
                fprintf(stderr, "-- Class 2\n");
            fprintf(stderr, "- Coloring in %f seconds\n", tie);
        );
    } else {
        cie = 0;
        tie = 0.0;
#endif
        if (output != NULL) {
            if(ci1 < ci2)
                in = openfile(fileH1, READ_MODE);
            else
                in = openfile(fileH2, READ_MODE);
            g.load_colorized(in);
            closefile(in);
        }
#ifndef COMPLETE_GRAPH
    }
#endif

    if (output != NULL)
        save(output, &g);

    /* Coloring (example simple greed)*/
    start = clock();
    g.set_edge_colors(0);
    cig = g.edge_colorize_greedy(1);
    end = clock();
    tig = timediff(end, start);
    INFO(
        fprintf(stderr, "#Chromatic index (greed) = %d\n", cig);
        if (cig == g.getDegree())
            fprintf(stderr, "-- Class 1\n");
        else
            fprintf(stderr, "-- Class 2\n");
        fprintf(stderr, "- Coloring in %f seconds\n", tig);
    );

    analyse(filename, &g, cie, tie, ci1, ti1, ci2, ti2, cig, tig);

    /* todo: deallocate variables */
    
    INFO(fprintf(stderr, "#-----------------------------------------------\n"));
    return EXIT_SUCCESS;
}
