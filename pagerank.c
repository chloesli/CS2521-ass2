// Reads Data from a given collection of pages in the file collection.txt

// Builds a graph structure using adjacency Matrix or List Representation

// Calculate weighted pagerank for every url in collection.txt

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "LList.h"
#include "Graph.h"
#include "pagerank.h"
#include "ReadData.h"

#define MAXURLLEN 1000

// ==== COMPILING ====
// gcc -Wall -Werror -std=gnu99 -O -lm -o pagerank pagerank.c ReadData.c LList.c Graph.c -g
void calculatePageRank(Graph g, double d, double diffPR, int maxIterations);
double getSum(Graph g, int vertex, double **PR, int iteration, int *outlinks, int *inlinks);
double getWin(Graph g, int pI, int pJ, int *inlinks);
double getWout(Graph g, int pI, int pJ, int *outlinks);

void getOutlinks(Graph g, int *outlinks);
void getInlinks(Graph g, int *inlinks);
int main(int argc, char *argv[]) {

    if (argc != 4) {
        fprintf(stderr, "Usage: %s <d - damping factor> <diffPR - difference in PageRank sum> <maxIterations - maximum iterations>\n", argv[0]);
        return 1;
    }
    
    
    FILE *data;
    
    
    if ((data = fopen("collection.txt","r")) == NULL) {
	    fprintf(stderr, "Couldn't open file: collection.txt");
	    return 1;
    }
    fclose(data); 

    LList urlList = GetCollection();
    printf("%d\n", urlList->nitems);
    struct LListNode *curr;
    for (curr = urlList->first; curr != NULL; curr=curr->next) {
        printf("%s\n", curr->value);
    }
    
    printf("\n");
    Graph g = GetGraph(urlList); 
    
    showGraph(g, 1);
    printf("\n");
    showGraph(g, 0);
    printf("\n");

    double d = atof(argv[1]);
    double diffPR = atof(argv[2]); 

    int maxIterations = atoi(argv[3]);
    
    calculatePageRank(g, d, diffPR, maxIterations);
  return 0;
}

void calculatePageRank(Graph g, double d, double diffPR, int maxIterations){
    // Array containing number of outlinks 
    int *outlinks = malloc(sizeof(int) * g->nV);
    getOutlinks(g, outlinks);
    int *inlinks = malloc(sizeof(int) * g->nV);
    getInlinks(g, inlinks);
        int x = 0;
    for (x = 0; x < g->nV; x++) {
        printf("%s has: %d inlinks, %d outlinks\n", g->vertex[x], inlinks[x], outlinks[x]);
    }
    printf("\n");
    double N = g->nV;
    int i = 0;
    
    double **PR = malloc(sizeof(double) * N); 
    
    // For every URL, set PR to 1/N 
    for (i = 0; i < N; i++) {
        PR[i] = malloc(sizeof(double) * maxIterations); 
        PR[i][0] = 1/N;  
        
        printf("%.7f", PR[i][0]);
    }
    
    double mult = ((1 - d)/N) + d; 
    // Iterations 
    int it = 0;
    // Diff
    double diff = diffPR; 
    // Values of iteration 
    int t = 0; 
    float PageSum = 0; 
    // For every page
    for (i = 0; i < g->nV; i++) {
        diff = diffPR; 
        t = 0;
        it = 0;
        while ((it < maxIterations) && (diff >= diffPR)) {   
            t = it + 1; 
            diff = 0;
            // For every link that Pi is connected to
                PageSum = getSum(g, i, PR, it, outlinks, inlinks);
                PR[i][t] = mult * PageSum;
            
                diff = fabs(PR[i][t] - PR[i][it]);
           
            it++;
        }   
        printf("%s %d %.7f\n", g->vertex[i], outlinks[i], PR[i][it]);
        // Print out number of iterations
        printf("Iterations: %d=",it);
    }
    

   /*
   // int v = 0;
    while ((it < maxIterations) && (diff >= diffPR)) {   
        t = it + 1; 
        diff = 0;
        // For every link that Pi is connected to
        for (i = 0; i < g->nV; i++) {
            PageSum = getSum(g, i, PR, it, outlinks, inlinks);
            PR[i][t] = mult * PageSum;

            diff = fabs(PR[i][t] - PR[i][it]);
        }
        it++;
    }   
    printf("--%d\n", it);

   for (i = 0; i < g->nV; i++) {
        printf("%s %d %.7f\n", g->vertex[i], outlinks[i], PR[i][it]);
   }
   */
   free(outlinks);
   free(inlinks);
    
    
    // Pi is what vertex is pointing to 
    // Pj is what Pi is pointing to (will multiple Pj's)
    // Setting all pageranks to 1/N 
    
    // Next iteration of the pagerank 
    // d is the damp 
    // M = size 
    // Sum of For all of Pj that is connected to Pi 
    // Everything connected to Pi is Pj = Outlinks of Pj are references 
    // Count inlinks of references (whats pointing to) 
    
    // Wout is the outlinks 
}

double getSum(Graph g, int vertex, double **PR, int iteration, int *outlinks, int *inlinks) {
    // For every link that Pi is connected to
    
    int i;
    double sum = 0; 
    // For every vertex
    for (i = 0; i < outlinks[vertex]; i++) {
        if (g->edges[vertex][i]) {
            // If there is an outlink from the vertex
            sum += PR[i][iteration] * getWin(g, vertex, i, inlinks) * getWout(g, vertex, i, outlinks);
        }
    }

    return sum;
}

double getWin(Graph g, int pI, int pJ, int *inlinks) {
    // Win is the number of outlinks 
    double win; 
    double denom = 0;
    int i; 
    for (i = 0; i < g->nV; i++) {
        // I points to a page
        if (g->edges[i][pI]) {
            denom += inlinks[i];
        }
    }
    win = inlinks[pJ] / denom; 
    // divided by the sum of Inlinks for pages pI points to
    return win;
}

double getWout(Graph g, int pI, int pJ, int *outlinks) {
    // Wout is the number of outlinks 
    double wout; 
    double denom = 0;
    int i; 
    // If pI has an outlink to i
    for (i = 0; i < g->nV; i++) {
        if (g->edges[i][pI]) {
            denom += outlinks[i];
        }
    }
    wout = outlinks[pJ] / denom; 
    // divided by the sum of Inlinks for pages pI points to
    return wout;
}

void getOutlinks(Graph g, int *outlinks) {
    int count = 0;
    int i; 
    int j;
        // For a vertex i
    for (i = 0; i < g->nV; i++) {
        for (j = 0; j < g->nV; j++) {
            if (g->edges[i][j])
	            count++;
        }
        outlinks[i] = count;
        count = 0;
    }
}

void getInlinks(Graph g, int *inlinks) {
    int count = 0;
    int i; 
    int j;
        // For a vertex i
    for (i = 0; i < g->nV; i++) {
        for (j = 0; j < g->nV; j++) {
            if (g->edges[j][i])
	            count++;
        }
        inlinks[i] = count;
        count = 0;
    }
}

// Create a list of urls 
