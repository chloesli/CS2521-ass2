// graph.h ... Interface to Graph of strings
// Taken from CS2521 S2 Week8 Lab
// Written by John Shepherd, September 2015
#ifndef GRAPH_H
#define GRAPH_H
typedef unsigned char Num;

typedef struct GraphRep {
	int   nV;
	int   maxV;
	char  **vertex;
	Num   **edges;
} GraphRep;

typedef struct GraphRep *Graph;

// Function signatures

Graph newGraph(int);
void  disposeGraph(Graph);
int   addEdge(Graph,char *,char *);
int   nVertices(Graph);
int   isConnected(Graph, char *, char *);
void  showGraph(Graph,int);

#endif
