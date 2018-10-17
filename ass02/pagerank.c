// Reads Data from a given collection of pages in the file collection.txt

// Builds a graph structure using adjacency Matrix or List Representation 

// Calculate weighted pagerank for every url in collection.txt

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "Graph.h"
#include "pagerank.h"

#define MAXURLLEN 1000

// ==== COMPILING ====  
// gcc -Wall -Werror -std=gnu99 -O -lm -o pagerank pagerank.c -g
int main(int argc, char *argv[]) {
    FILE *data; 
    
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <d - damping factor> <diffPR - difference in PageRank sum> <maxIterations - maximum iterations>\n", argv[0]);
        return 1;
    }
  
    if ((data = fopen("ex1/collection.txt","r")) == NULL) {
	    fprintf(stderr, "Couldn't open file: collection.txt");
	    return 1;
    }
    fclose(data);
    
    Li urlList = ListofUrls();
    printf("%d\n", urlList->nvalues);
    struct ListNode *curr;
    for (curr = urlList->first; curr != NULL; curr=curr->next) {
        printf("%s\n", curr->value); 
    }   
    
    
  return 0;
}

// Create a list of urls 

Li ListofUrls() {
   // printf("%d\n",nurls);
   
    FILE *urls = fopen("ex1/collection.txt","r");
   
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

static ListNode *MakeNode(char *val) {
	ListNode *new;
	new = malloc(sizeof(ListNode));
	assert(new != NULL);
	new->value = strdup(val);
	new->prev = new->next = NULL;
	return new;
}

