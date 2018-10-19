// By... Midog, chlodog, Berndog

#ifndef READDATA_H
#define READDATA_H

#include "LList.h"

// GetCollection()
// -> Create a set (list) of urls to process by reading data from file collection.txt


// function declarations

// GetCollection
LList GetCollection();

Graph GetGraph(LList urls);



// GetGraph(List_of_urls)
// Create empty graph (use graph ADT in say graph.h and graph.c)
// For each url in the above list
// read <url>.txt file, and update graph by adding a node and outgoing links


// GetInvertedList(List_of_urls)
// Create empty inverted list (use say List of lists, BST where values are lists, etc)
// For each url in List_of_Urls
// read <url>.txt file, and update inverted index


#endif
