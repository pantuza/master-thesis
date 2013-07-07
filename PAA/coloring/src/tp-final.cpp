/**
 * tp2 program
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

void save(const char *name, Graph::Graph *g)
{
    clock_t start;
    clock_t end;
    char fileName[1024];
    /* Export graph in DOT*/
    start = clock();
    sprintf(fileName, "out/%s.dot", name);
    FILE *dot = openfile(fileName, WRITE_MODE);
    g->save2dot(dot);
    closefile(dot);
    end = clock();
    INFO(fprintf(stderr, "Export DOT in %f seconds: %s\n",
            timediff(end, start), fileName));

    /* exports to output file */
    start = clock();
    sprintf(fileName, "out/%s.txt", name);
    FILE *out = openfile(fileName, WRITE_MODE);
    g->save(out);
    closefile(out);
    end = clock();
    INFO(fprintf(stderr, "Export graph in %f seconds: %s\n",
        timediff(end, start), fileName));
}

int main(int argc, char *argv[])
{
    //setvbuf(stdout, NULL, _IOFBF, FILE_BUFFER_SIZE);
    setvbuf(stderr, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);

    // Program execution options
    
    /* Timers */
    clock_t start;
    clock_t end;
    int ci;
    Graph::Graph g;
    
    /* Imports input file */
    start = clock();
    char fileIn[] = "in/g.txt";
    FILE *in = openfile(fileIn, READ_MODE);
    g.load(in);
    closefile(in);
    end = clock();
    INFO(
        fprintf(stderr, "## Graph degree = %d:\n", g.getDegree());
        fprintf(stderr, "--Import in %f seconds\n", timediff(end, start))
    );

    // Coloring (heuristic 1)
    start = clock();
    FILE *inBase1 = openfile(fileIn, READ_MODE);
    ci = baseline1::misraGriesNewBaseline(inBase1);
    closefile(inBase1);
    end = clock();
    INFO(
        fprintf(stderr, "#Chromatic index (heuristic misraGriesNew) = %d\n", ci);
        if (ci == g.getDegree())
            fprintf(stderr, "-- Class 1\n");
        else
            fprintf(stderr, "-- Class 2\n");
        fprintf(stderr, "- Coloring in %f seconds\n", timediff(end, start));
    );

    // Coloring (heuristic 2)
    start = clock();
    FILE *inBase2 = openfile(fileIn, READ_MODE);
    ci = baseline2::edgeColor4Baseline(inBase2);
    closefile(inBase2);
    end = clock();
    INFO(
        fprintf(stderr, "#Chromatic index (heuristic edgeColor4) = %d\n", ci);
        if (ci == g.getDegree())
            fprintf(stderr, "-- Class 1\n");
        else
            fprintf(stderr, "-- Class 2\n");
        fprintf(stderr, "- Coloring in %f seconds\n", timediff(end, start));
    );

    // Coloring (baseline exact)
    start = clock();
    g.set_edge_colors(0);
    ci = g.edge_colorize_bf(1);
    end = clock();
    INFO(
        fprintf(stderr, "#Chromatic index (baseline brute force) = %d\n", ci);
        if (ci == g.getDegree())
            fprintf(stderr, "-- Class 1\n");
        else
            fprintf(stderr, "-- Class 2\n");
        fprintf(stderr, "- Coloring in %f seconds\n", timediff(end, start));
    );

    save("g", &g);

    /* Coloring (example simple greed)*/
    start = clock();
    g.set_edge_colors(0);
    ci = g.edge_colorize_greedy(1);
    end = clock();
    INFO(
        fprintf(stderr, "#Chromatic index (greed) = %d\n", ci);
        if (ci == g.getDegree())
            fprintf(stderr, "-- Class 1\n");
        else
            fprintf(stderr, "-- Class 2\n");
        fprintf(stderr, "- Coloring in %f seconds\n", timediff(end, start));
    );

    save("g_h", &g);
    
    /* deallocate variables */
    
    return EXIT_SUCCESS;
}
