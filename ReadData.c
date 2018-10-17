// By...Midog, chlodog, Berndog

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "Graph.h"
#include "pagerank.h"
#include "ReadData.h"
// Implementations

// GetCollection()
// -> Create a set (list) of urls to process by reading data from file collection.txt
Li GetCollection() {
   // printf("%d\n",nurls);

    FILE *urls = fopen("TestExamples/ex1/collection.txt","r");

    Li new = malloc(sizeof(struct List));
    new->nvalues = 0;

    struct ListNode *newNode;
    struct ListNode *curr = NULL;
    char val[1000];

    while( fscanf(urls, "%s", val) == 1){
        newNode = MakeNode(val);

        if (new->nvalues == 0) {
            new->first = newNode;
            newNode->prev = NULL;
            curr = newNode;
        }
        // If the text buffer has more than 0 Lines
        else {

            curr->next = newNode;
            curr->next->prev = curr;
            curr = curr->next;
        }

        new->last = newNode;
        new->last->next = NULL;
        new->nvalues++;

    }

    fclose(urls);
    return new;
    // hi this is a new file
}

ListNode *MakeNode(char *val) {
	ListNode *new;
	new = malloc(sizeof(ListNode));
	assert(new != NULL);
	new->value = strdup(val);
	new->prev = new->next = NULL;
	return new;
}

// GetGraph(List_of_urls)
// Create empty graph (use graph ADT in say graph.h and graph.c)
// For each url in the above list
// read <url>.txt file, and update graph by adding a node and outgoing links


// GetInvertedList(List_of_urls)
// Create empty inverted list (use say List of lists, BST where values are lists, etc)
// For each url in List_of_Urls
// read <url>.txt file, and update inverted index
