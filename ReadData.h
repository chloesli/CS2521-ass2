// By... Midog, chlodog, Berndog

#ifndef READDATA_H
#define READDATA_H

#include "LList.h"
#include "Graph.h"
#include "inverted.h"
#include "ReadData.h"

#define TRUE 1
#define FALSE 0

// Create a set (list) of urls to process by reading data from file collection.txt
LList GetCollection();

// Create empty graph (use graph ADT in say graph.h and graph.c)
// For each url in the above list
// read <url>.txt file, and update graph by adding a node and outgoing links
Graph GetGraph(LList urls);

#endif
