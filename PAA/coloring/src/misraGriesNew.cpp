// Prototype Vizing (degree+1) edge colorer
// See Misra & Gries, "A constructive proof of Vizing's Theorem",
// IPL 41 (1992) 131-133.

// 4/23/03 - rejects multigraphs
// 4/25/03 - linked lists to allow faster construction of fan.
// 5/12/04 - simplified to correspond to CSE 5311 notes on NP-completeness

#include <stdlib.h>
#include <stdio.h>
#include "debug.h"

namespace H1 {


    struct edge {
            int firstVertex,secondVertex,color;
    };
    typedef struct edge edgeType;
    edgeType edgeTab[10000];

    int vertexDegree[1000];
    int vertexAcrossColor[1000][1002];  // Given a vertex number and a color,
    // indicates adjacent vertex (-1=none)
    int prev[1000][1002], // Used with vertexAcrossColor for
    next[1000][1002], // circular doubly-linked lists of available colors.
    firstFree[1000]; // Points to header of list.

    int m,n,degree;

    int firstThenSecond(const void* xin, const void* yin)
    // Used in call to qsort()
    {
        int result;
        edgeType *x,*y;

        x=(edgeType*) xin;
        y=(edgeType*) yin;
        result=x->firstVertex - y->firstVertex;
        if (result!=0)
            return result;
        else
            return x->secondVertex - y->secondVertex;
    }

    void setColor(int vertexNum1,int vertexNum2,int edgeColor)
    {
        int n,p; // next node, current (deleted) node, previous node

        DEBUG1(printf("debug - setColor(%d,%d,%d)\n",vertexNum1,vertexNum2,edgeColor));
        if (vertexAcrossColor[vertexNum1][edgeColor]!=(-1))
        {
            printf("Fatal - attempt at reusing a color at vertex %d\n",vertexNum1);
            exit(0);
        }
        if (vertexAcrossColor[vertexNum2][edgeColor]!=(-1))
        {
            printf("Fatal - attempt at reusing a color at a vertex %d\n",vertexNum2);
            exit(0);
        }
        // Actual updating, including 2 deletions from circular doubly-linked lists.
        vertexAcrossColor[vertexNum1][edgeColor]=vertexNum2;
        //c=edgeColor;
        n=next[vertexNum1][edgeColor];
        p=prev[vertexNum1][edgeColor];
        prev[vertexNum1][n]=p;
        next[vertexNum1][p]=n;

        vertexAcrossColor[vertexNum2][edgeColor]=vertexNum1;
        n=next[vertexNum2][edgeColor];
        p=prev[vertexNum2][edgeColor];
        prev[vertexNum2][n]=p;
        next[vertexNum2][p]=n;
    }

    void clearColor(int vertexNum1,int vertexNum2,int edgeColor)
    {
        int n,c,p; // next node, current (inserted) node, previous node

        DEBUG1(printf("debug - clearColor(%d,%d,%d)\n",vertexNum1,vertexNum2,edgeColor));
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
        // Actual updating, including 2 insertions after headers of
        // circular doubly-linked lists.
        vertexAcrossColor[vertexNum1][edgeColor]=(-1);
        c=edgeColor;
        p=firstFree[vertexNum1];
        n=next[vertexNum1][firstFree[vertexNum1]];
        prev[vertexNum1][c]=p;
        next[vertexNum1][c]=n;
        next[vertexNum1][p]=c;
        prev[vertexNum1][n]=c;

        vertexAcrossColor[vertexNum2][edgeColor]=(-1);
        p=firstFree[vertexNum2];
        n=next[vertexNum2][firstFree[vertexNum2]];
        prev[vertexNum2][c]=p;
        next[vertexNum2][c]=n;
        next[vertexNum2][p]=c;
        prev[vertexNum2][n]=c;
    }

    void colorEdge(int X, int f)
    {
        int i,k,c,d;
        int dcArray[100];
        int fan[101],fanFreeColor[101],fanSize,colorInFan[101];
        DEBUG1(int w);

        DEBUG1(printf("debug - test easy case: X=%d, f=%d\n",X,f));
        // Faster code for finding common free color for X and f
        for (c=next[X][firstFree[X]];
                c!=degree+1; // Looped back to header?
                c=next[X][c])
            if (vertexAcrossColor[f][c]==(-1))
            {
                setColor(X,f,c);
                return;
            }

        //Build fan
        DEBUG1(printf("Fan for X=%d, f=%d\n",X,f));
        fanSize=1;
        fan[0]=f;
        DEBUG1(printf("%d ",fan[0]));
        for (i=0;i<=degree;i++)
            colorInFan[i]=0;
        while (1)
        {
            // This for-loop has been replaced by circular doubly-linked lists that allow
            // finding a free color in O(1) time instead of O(V), but actual speed-up
            // is doubtful.
            //
            //for (i=0;i<=degree;i++)
            //    if (vertexAcrossColor[fan[fanSize-1]][i]==(-1))
            //        break;
            //
            k=fan[fanSize-1];
            i=next[k][firstFree[k]];
            if (colorInFan[i] || vertexAcrossColor[X][i]==(-1))
            {
                d=i;
                break;
            }
            fanFreeColor[fanSize-1]=i;
            DEBUG1(printf("(%d) ",i));
            colorInFan[i]=1;
            fanSize++;
            fan[fanSize-1]=vertexAcrossColor[X][i];
            DEBUG1(printf("%d ",fan[fanSize-1]));
        }
        DEBUG1(printf("\n"));
        DEBUG1(printf("debug - d=%d\n",d));

        // Added 5/12/04 to correspond to CSE 5311 notes
        if (vertexAcrossColor[X][d]==(-1))
        {
            //Rotate fan
            DEBUG1(printf("rotating fan - without truncation\n"));
            for (k=1;k<fanSize;k++)
                clearColor(X,fan[k],fanFreeColor[k-1]);
            fanFreeColor[fanSize-1]=d;
            for (k=0;k<fanSize;k++)
                setColor(X,fan[k],fanFreeColor[k]);
            return;
        }

        // Replaced below by finding c in O(1)
        //for (c=0;c<=degree;c++)
        //    if (vertexAcrossColor[X][c]==(-1))
        //        break;
        //if (c>degree)
        //{
        //    printf("Fatal - could not set c\n");
        //    exit(0);
        ///}
        //
        c=next[X][firstFree[X]];
        DEBUG1(printf("debug - c=%d\n",c));

        //Find the dc-path
        DEBUG1(printf("debug - finding dc-path\n"));
        dcArray[0]=X;
        k=0;
        while (1)
        {
            if (k%2==0)
                i=vertexAcrossColor[dcArray[k]][d];
            else
                i=vertexAcrossColor[dcArray[k]][c];
            if (i==(-1))
                break;
            k++;
            dcArray[k]=i;
        }

        if (k==0)
        {
            printf("error - empty dc-path\n");
            exit(0);
        }
        //Invert the dc-path
        DEBUG1(printf("debug - inverting dc-path\n"));
        for (i=0;i<k;i+=2)
            clearColor(dcArray[i],dcArray[i+1],d);
        for (i=1;i<k;i+=2)
            clearColor(dcArray[i],dcArray[i+1],c);
        for (i=0;i<k;i+=2)
            setColor(dcArray[i],dcArray[i+1],c);
        for (i=1;i<k;i+=2)
            setColor(dcArray[i],dcArray[i+1],d);

        //Patch fan and find w
        for (i=1;i<fanSize;i++)
            if (vertexAcrossColor[X][fanFreeColor[i-1]]!=fan[i])
            {
                if (fanFreeColor[i-1]==d)
                {
                    // This allows edge to have color cleared correctly, if needed.
                    fanFreeColor[i-1]=c;
                    DEBUG1(printf("c patch vertex %d to vertex %d\n",X,fan[i]));
                    if (vertexAcrossColor[fan[i-1]][d]==(-1))
                    {
                        // Fan should be truncated
                        DEBUG1(printf("Fan truncated\n"));
                        i--;
                    }
                    else
                    {
                        // Unusual dc-path.  First edge (colored d before inverting)
                        // is followed by an edge (colored c before inverting)
                        // to the predecessor of the d-edge in the fan.
                        DEBUG1(printf("Unusual dc-path\n"));
                        i=fanSize-1;
                    }
                    break;
                }
                else
                {
                    printf("Fatal patching fan\n");
                    exit(0);
                }
            }
        if (i==fanSize)
        {
            printf("Fatal - didn't find d edge in fan\n");
            exit(0);
        }
        DEBUG1(
                w=fan[i];
                printf("w is %d\n",w);
        );

        //Rotate fan
        DEBUG1(printf("rotating fan - after truncation\n"));
        for (k=1;k<=i;k++)
            clearColor(X,fan[k],fanFreeColor[k-1]);
        fanFreeColor[i]=d;
        for (k=0;k<=i;k++)
            setColor(X,fan[k],fanFreeColor[k]);
    }

    int edge_coloring(FILE *file, FILE *out)
    {
        int i,j,k;
        int v,w;

        fscanf(file, "%d %d",&n,&m);
        for (i=0;i<n;i++)
            vertexDegree[i]=0;
        for (i=0;i<m;i++)
        {
            fscanf(file, "%d %d",&v,&w);
            if (v<0 || w<0 || v>=n || w>=n)
            {
                printf("Fatal: bad vertex\n");
                exit(0);
            }
            else if (v==w)
            {
                printf("Fatal: self-loop\n");
                exit(0);
            }
            else if (v>w)
            {
                edgeTab[i].firstVertex=w;
                edgeTab[i].secondVertex=v;
            }
            else
            {
                edgeTab[i].firstVertex=v;
                edgeTab[i].secondVertex=w;
            }
            vertexDegree[v]++;
            vertexDegree[w]++;
        }

        qsort(edgeTab,m,sizeof(edgeType),firstThenSecond);
        for (i=1;i<m;i++)
            if (edgeTab[i].firstVertex==edgeTab[i-1].firstVertex
                    && edgeTab[i].secondVertex==edgeTab[i-1].secondVertex)
            {
                printf("Fatal: duplicate edge\n");
                exit(0);
            }

        degree=vertexDegree[0];
        for (i=1;i<n;i++)
            if (degree<vertexDegree[i])
                degree=vertexDegree[i];

        for (i=0;i<n;i++)
        {
            // vertexAcrossColor[i][j] indicates vertex on other side
            // of edge colored j from vertex i.  -1 indicates that
            // color is unused.  Unused colors are linked together
            // to speed-up (?) finding a fan
            for (j=0;j<=degree;j++)
            {
                vertexAcrossColor[i][j]=(-1);
                prev[i][j]=j-1;
                next[i][j]=j+1;
            }
            // degree+1 is the dummy header node of circular doubly-linked list.
            firstFree[i]=degree+1;
            prev[i][degree+1]=degree;
            next[i][degree+1]=0;
            vertexAcrossColor[i][degree+1]=(-9999999); //Blow up anything using this
            prev[i][0]=degree+1;
            next[i][degree]=degree+1;
        }

        for (i=0;i<m;i++)
            colorEdge(edgeTab[i].firstVertex,edgeTab[i].secondVertex);

        int colors[100];
        for(i=0;i<100;i++)
            colors[i] = 0;

        //Output results
        //Writes over edgeTab
        k=0;
        for (i=0;i<n;i++)
            for (j=0;j<=degree;j++)
                if (vertexAcrossColor[i][j]>i)
                {
                    edgeTab[k].firstVertex=i;
                    edgeTab[k].secondVertex=vertexAcrossColor[i][j];
                    edgeTab[k].color=j;
                    colors[j]++;
                    k++;
                }
        INFO(
            if (k!=m)
                printf("Uncolored edge?\n");
        );

        qsort(edgeTab,k,sizeof(edgeType),firstThenSecond);
        DEBUG1(
            printf("Edge & color\n");
            for (i=0;i<k;i++)
                printf("{%d, %d} %d\n",edgeTab[i].firstVertex,
                        edgeTab[i].secondVertex,edgeTab[i].color);
        );
        if (out != NULL) {
            fprintf(out, "%d %d\n", n,m);
            for (i=0;i<k;i++)
                fprintf(out, "%d %d %d\n",edgeTab[i].firstVertex,
                        edgeTab[i].secondVertex,edgeTab[i].color);
        }

        int numColors = 0;
        for(i=0;i<100;i++)
            if(colors[i] > 0)
                numColors++;
        return numColors;
    }
}
