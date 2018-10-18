// Reads Data from a given collection of pages in the file collection.txt

// Builds a graph structure using adjacency Matrix or List Representation

// Calculate weighted pagerank for every url in collection.txt

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "LList.h"
#include "Graph.h"
#include "pagerank.h"
#include "ReadData.h"

#define MAXURLLEN 1000

// ==== COMPILING ====
// gcc -Wall -Werror -std=gnu99 -O -lm -o pagerank pagerank.c ReadData.c LList.c -g
int main(int argc, char *argv[]) {

    if (argc != 4) {
        fprintf(stderr, "Usage: %s <d - damping factor> <diffPR - difference in PageRank sum> <maxIterations - maximum iterations>\n", argv[0]);
        return 1;
    }
    
    
    FILE *data;
    
    
    if ((data = fopen("TestExamples/ex1/collection.txt","r")) == NULL) {
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


  return 0;
}

// Create a list of urls
