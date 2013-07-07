// Prototype Vizing (degree+1) edge colorer
// See Hu and Blake, "Algorithms for Scheduling with
// Applications to Parallel Computing", Advances in
// Engineering Software 28, 563-572, 1997.

// Efficient - no adjacency matrix
// This version does not scan in step 4
// Takes mn time, but is unpredictable if duplicate edges are in input

#include <stdio.h>
#include <stdlib.h>

#include "debug.h"

namespace baseline2 {

    int firstVertex[10000],secondVertex[10000];
    int vertexDegree[100];
    int vertexAcrossColor[100][101];  // Given a vertex number and a color,
    // indicates adjacent vertex (-1=none)
    int m,n,degree;

    void setColor(int vertexNum1,int vertexNum2,int edgeColor)
    {
        DEBUG1(printf("debug - setColor(%d,%d,%d)\n",vertexNum1,vertexNum2,edgeColor));
        if (vertexAcrossColor[vertexNum1][edgeColor]!=(-1))
        {
            printf("Fatal - attempt at reusing a color at a vertex\n");
            exit(0);
        }
        if (vertexAcrossColor[vertexNum2][edgeColor]!=(-1))
        {
            printf("Fatal - attempt at reusing a color at a vertex\n");
            exit(0);
        }
        vertexAcrossColor[vertexNum1][edgeColor]=vertexNum2;
        vertexAcrossColor[vertexNum2][edgeColor]=vertexNum1;
    }

    void clearColor(int vertexNum1,int vertexNum2,int edgeColor)
    {
        DEBUG1(printf("debug - clearColor(%d,%d,%d)\n",
                vertexNum1,vertexNum2,edgeColor));
        if (vertexAcrossColor[vertexNum1][edgeColor]!=vertexNum2)
        {
            printf("Fatal - attempt to clear wrong color\n");
            exit(0);
        }
        if (vertexAcrossColor[vertexNum2][edgeColor]!=vertexNum1)
        {
            printf("Fatal - attempt to clear wrong color\n");
            exit(0);
        }
        vertexAcrossColor[vertexNum1][edgeColor]=(-1);
        vertexAcrossColor[vertexNum2][edgeColor]=(-1);
    }

    void Step3(int v, int w)
    {
        int i,k,c,c1,c2,c3;
        int wArray[100];

        DEBUG1(printf("debug - step 3: v=%d, w=%d\n",v,w));

        for (c=1;c<=degree+1;c++)
            if (vertexAcrossColor[v][c]==(-1) && vertexAcrossColor[w][c]==(-1))
                break;
        if (c<=degree+1)
        {
            setColor(v,w,c);
            return; // back to Step 2 loop
        }

        for (c1=1;
                c1<=degree+1 && vertexAcrossColor[v][c1]>(-1);
                c1++)
            ;
        if (c1>degree+1)
        {
            printf("Fatal 1\n");
            exit(0);
        }
        DEBUG1(printf("debug - c1=%d\n",c1));
        for (c2=1;
                c2<=degree+1 && vertexAcrossColor[w][c2]>(-1);
                c2++)
            ;
        if (c2>degree+1)
        {
            printf("Fatal 2\n");
            exit(0);
        }
        DEBUG1(printf("debug - c2=%d\n",c2));

        while (1)
        {
            // Step 4

            DEBUG1(printf("debug - step 4\n"));

            wArray[0]=w;
            k=0;
            while (1)
            {
                DEBUG1(printf("debug - step 4:  searching for k=%d wArray[%d]=%d\n",k,k,wArray[k]));
                if (k%2==0)
                    i=vertexAcrossColor[wArray[k]][c1];
                else
                    i=vertexAcrossColor[wArray[k]][c2];
                if (i==(-1))
                    break;
                k++;
                wArray[k]=i;
            }
            if (v!=wArray[k])
            {
                // Step 5

                DEBUG1(printf("debug - step 5\n"));

                for (i=0;i<k;i+=2)
                    clearColor(wArray[i],wArray[i+1],c1);
                for (i=1;i<k;i+=2)
                    clearColor(wArray[i],wArray[i+1],c2);
                setColor(v,w,c1);
                for (i=0;i<k;i+=2)
                    setColor(wArray[i],wArray[i+1],c2);
                for (i=1;i<k;i+=2)
                    setColor(wArray[i],wArray[i+1],c1);
                return; // Back to Step 2 loop
            }

            // Step 6

            DEBUG1(printf("debug - step 6\n"));

            for (c3=1;c3<=degree+1;c3++)
                if (vertexAcrossColor[v][c3]==(-1) &&
                        vertexAcrossColor[wArray[k-1]][c3]==(-1))
                    break;
            if (c3<=degree+1)
            {
                clearColor(v,wArray[k-1],c2);
                setColor(v,w,c2);
                setColor(v,wArray[k-1],c3);
                return; // Back to Step 2 loop
            }

            // Step 7

            DEBUG1(printf("debug - step 7\n"));

            for (c3=1;c3<=degree+1;c3++)
                if (vertexAcrossColor[wArray[k-1]][c3]==(-1))
                    break;
            if (c3>degree+1)
            {
                printf("Fatal 3\n");
                exit(0);
            }
            clearColor(v,wArray[k-1],c2);
            setColor(v,w,c2);
            w=wArray[k-1];
            c2=c3;
        }
    }

    int edgeColor4Baseline(FILE *file)
    {
        int i,j;

        fscanf(file, "%d %d",&n,&m);
        for (i=0;i<n;i++)
            vertexDegree[i]=0;
        for (i=0;i<m;i++)
        {
            fscanf(file, "%d %d",&firstVertex[i],&secondVertex[i]);
            vertexDegree[firstVertex[i]]++;
            vertexDegree[secondVertex[i]]++;
        }

        //Step 1

        degree=vertexDegree[0];
        for (i=1;i<n;i++)
            if (degree<vertexDegree[i])
                degree=vertexDegree[i];

        for (i=0;i<n;i++)
            for (j=1;j<=degree+1;j++)
                vertexAcrossColor[i][j]=(-1);

        // Step 2
        for (i=0;i<m;i++)
            Step3(firstVertex[i],secondVertex[i]);

        int colors[100];
        for(i=0;i<100;i++)
            colors[i] = 0;

        //Output results
        DEBUG1(printf("Edge & color\n"));
        for (i=0;i<n;i++)
            for (j=1;j<=degree+1;j++)
                if (vertexAcrossColor[i][j]>i)
                {
                    DEBUG1(printf("%d %d %d\n",i,vertexAcrossColor[i][j],j));
                    colors[j]++;
                }

        int numColors = 0;
        for(i=0;i<100;i++)
            if(colors[i] > 0)
                numColors++;
        return numColors;

    }
}
