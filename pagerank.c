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
#include "ReadData.h"

#define MAXURLLEN 1000

/************************************************************
  FUNCTION DECLARATIONS
************************************************************/

void calculatePageRank(Graph g, double d, double diffPR, int maxIterations);
double getSum(Graph g, int vertex, double **PR, int iteration, int *outlinks, int *inlinks);
double getWin(Graph g, int pI, int pJ, int *inlinks);
double getWout(Graph g, int pI, int pJ, int *outlinks);
void getOutlinks(Graph g, int *outlinks);
void getInlinks(Graph g, int *inlinks);
int getLargest(double **PR, int *checked, int N, int it);

int main(int argc, char *argv[]) {
    // If the number of arguments provided is not enough, print out an error message
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <d - damping factor> <diffPR - difference in PageRank sum> <maxIterations - maximum iterations>\n", argv[0]);
        return 1;
    }
    
    // If there collection.txt is NULL, print out an error message 
    FILE *data;
    if ((data = fopen("collection.txt","r")) == NULL) {
	    fprintf(stderr, "Couldn't open file: collection.txt");
	    return 1;
    }
    fclose(data); 
    // Gets a linked list of URLS from collection.txt
    LList urlList = GetCollection();
    // Reads each url file and creates a graph
    Graph g = GetGraph(urlList); 
    // Gets the dampening factor, difference in PageRank sum and max iterations from command line arguments
    double d = atof(argv[1]);
    double diffPR = atof(argv[2]); 
    int maxIterations = atoi(argv[3]);
    // Calculates PageRank
    calculatePageRank(g, d, diffPR, maxIterations);
    
    // Free memory associated with Graph
    disposeGraph(g);
    return 0;
}

void calculatePageRank(Graph g, double d, double diffPR, int maxIterations){
    // Creating and initialising arrays that contain the number of inlinks and outlinks of each page
    int *outlinks = malloc(sizeof(int) * g->nV);
    getOutlinks(g, outlinks);
    int *inlinks = malloc(sizeof(int) * g->nV);
    getInlinks(g, inlinks);
    
    /************************************************************
     CALCULATE PAGERANK 
    ************************************************************/
   
    // N = number of URLs in collection
    double N = g->nV;
    // Mallocing space for a matrix to store values of PageRank
    double **PR = malloc(sizeof(double) * N); 
    // For every URL, set the initial PR to 1/N 
    int i = 0;
    for (i = 0; i < N; i++) {
        PR[i] = malloc(sizeof(double) * (maxIterations + 1)); 
    }
    
    double diff = diffPR;
    double mult = ((1 - d)/N); 
    int it = 0;
    int t = 0; 
    int v = 0;
    
    // Loop while iteration is less than maxIterations and diff is greater than the min difference in PageRank sum
    while ((it < maxIterations) && (diff >= diffPR)) {  
        // t = iteration number + 1
        t = it + 1; 
        // Calculate the PageRank for every page
        for (i = 0; i < N; i++) {
            // PageRank = (1 - d)/N + d * (For all reference pages of i (pJ), the sum of: PR[pJ][t] * WeightedInput[pJ][pI] * WeightedOutput[pJ][pI])
            
            PR[i][t] = mult + (d * getSum(g, i, PR, it, outlinks, inlinks));
            
        }
        // Calculate the difference in PageRank sum
        for (v = 0; v < N; v++) {
                diff += fabs(PR[v][t] - PR[v][it]);
        }
        // Increment iteration
        it++;
    }   

    /************************************************************
     SORT PAGERANKS IN DESCENDING 
    ************************************************************/
    // Array to store the index of PageRanks in descending order
    int *order = calloc(N , sizeof(int));
    // Array to show if a value has been added to the order array
    int *checked = calloc(N, sizeof(int));

    int maxIndex = 0;
    int j = 0;
    for (j = 0; j < N; j++) {
        // getLargest returns the index value of the largest PageRank that has not been checked
        maxIndex = getLargest(PR, checked, N, it);
        order[j] = maxIndex;
    }
    
    /************************************************************
     WRITE PAGERANKS IN DESCENDING ORDER TO A FILE
    ************************************************************/
    FILE *fp = fopen("pagerankList.txt", "w");
    for (i = 0; i < N; i++) {
        fprintf(fp,"%s, %d, %.7f\n", g->vertex[order[i]], outlinks[order[i]], PR[order[i]][it]);
    }
    fclose(fp);
    
    /************************************************************
     FREE MEMORY ALLOCATED TO ARRAYS
    ************************************************************/
    free(outlinks);
    free(inlinks);
    free(order);
    free(checked);
    free(PR);
    
}

// Returns the index of the largest PageRank value that has not been checked 
int getLargest(double **PR, int *checked, int N, int it) {
    int i = 0;
    float max = 0; 
    int maxIndex = 0;
    
    for (i = 0; i < N; i++) {
        // If the PageRank value is greater than max and has not been checked
        if (PR[i][it] > max && checked[i] == 0) {
            max = PR[i][it];
            maxIndex = i;
        }
    }
    // Marked the largest to be returned as checked
    checked[maxIndex] = 1;
    
    return maxIndex; 
}

// Takes in a graph, a vertex, the PageRank matrix, iteration number and arrays containing inlinks and outlinks
// Returns the sum (float)
double getSum(Graph g, int vertex, double **PR, int iteration, int *outlinks, int *inlinks) {

    int i;
    double sum = 0; 
    // For every vertex
    for (i = 0; i < g->nV; i++) {
        // If there is an outlink from the vertex
        if (g->edges[i][vertex]) {
            sum += PR[i][iteration] * getWin(g, i, vertex, inlinks) * getWout(g, i, vertex, outlinks);
        }
    }

    return sum;
}

// Calculates the Weighted input of [pI][pJ]
double getWin(Graph g, int pI, int pJ, int *inlinks) {
    // Win is the number of outlinks 
    double win; 
    double denom = 0;
    int i; 
    // Calculate the denominator (sum of inlinks of outlinks of pI)
    for (i = 0; i < g->nV; i++) {
        // If there is an edge from pI to vertex[i]
        if (g->edges[pI][i]) {
            // Add the number of vertex[i]'s inlinks to the denominator
            denom += inlinks[i];
        }
    }
    
    // Divide the number of inlinks to pJ by the sum of inlinks for outlinks of pI

    win = inlinks[pJ] / denom; 
    
    return win;
}

// Calculates the Weighted output of [pI][pJ]
double getWout(Graph g, int pI, int pJ, int *outlinks) {
    // Wout is the number of outlinks 
    double wout; 
    double denom = 0;
    int i; 
    // Calculate the denominator (sum of outlinks of outlinks of pI)
    for (i = 0; i < g->nV; i++) {
        // If there is an edge from pI to vertex[i]
        if (g->edges[pI][i]) {
            // Add the number of vertex[i]'s outlinks to the denominator
            // If the number of outlinks is 0, let it be 0.5
            if (outlinks[i] == 0) {
                denom += 0.5;
            }
            denom += outlinks[i];
        }
    }
    // Divide the number of inlinks to pJ by the sum of inlinks for outlinks of pI
    // If the number of outlinks is 0, let it be 0.5
    if (outlinks[pJ] == 0) {
        wout = 0.5 / denom;
    } else { 
        wout = outlinks[pJ] / denom; 
    }

    return wout;
}

// Determine the number of outlinks for each vertex[i]
void getOutlinks(Graph g, int *outlinks) {
    int count = 0;
    int i; 
    int j;
    // For each vertex
    for (i = 0; i < g->nV; i++) {
        for (j = 0; j < g->nV; j++) {
            // If there is an edge between vertex[i] and vertex[j] increment count
            if (g->edges[i][j])
	            count++;
        }
        outlinks[i] = count;
        count = 0;
    }
}

// Determine the number of inlinks for each vertex[i]
void getInlinks(Graph g, int *inlinks) {
    int count = 0;
    int i; 
    int j;
    // For each vertex
    for (i = 0; i < g->nV; i++) {
        for (j = 0; j < g->nV; j++) {
        // If there is an edge between vertex[i] and vertex[j] increment count
            if (g->edges[j][i])
	            count++;
        }
        inlinks[i] = count;
        count = 0;
    }
}
