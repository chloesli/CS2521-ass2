// By... Midog, chlodog, Berndog

#ifndef READDATA_H
#define READDATA_H

#include "ReadData.h"
// GetCollection()
// -> Create a set (list) of urls to process by reading data from file collection.txt
typedef struct List *Li;

typedef struct ListNode {
    char *value;
    struct ListNode *next;
    struct ListNode *prev;
} ListNode;

typedef struct List{
    int nvalues;
    struct ListNode *first;
    struct ListNode *last;
} List;


// function declarations
ListNode *MakeNode(char *val);
Li GetCollection();

// GetGraph(List_of_urls)
// Create empty graph (use graph ADT in say graph.h and graph.c)
// For each url in the above list
// read <url>.txt file, and update graph by adding a node and outgoing links


// GetInvertedList(List_of_urls)
// Create empty inverted list (use say List of lists, BST where values are lists, etc)
// For each url in List_of_Urls
// read <url>.txt file, and update inverted index


#endif
