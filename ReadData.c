// By...Midog, chlodog, Berndog

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "Graph.h"
#include "pagerank.h"
#include "ReadData.h"
#include "LList.h"


// Implementations

// GetCollection()
// -> Create a set (list) of urls to process by reading data from file collection.txt

LList GetCollection() {
   // printf("%d\n",nurls);

    FILE *urls = fopen("collection.txt","r");

    LListNode *curr = NULL;
    LList new = newLList();
    char val[1000];

    while( fscanf(urls, "%s", val) == 1){
    
        LListNode *newNode = newLListNode(val);

        if (new->nitems == 0) {
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
        new->nitems++;

    }

    fclose(urls);
    return new;
    
}


// GetGraph(List_of_urls)
Graph GetGraph(LList urls) { // Makes a new empty graph 

    Graph g = newGraph(urls->nitems); 
    LListNode *curr; 
    FILE *page; 
    
    // temp is to append .txt without changing the original 
    char *temp; // dest is the word that you check if its the end of section 
    char *dest = calloc(sizeof(char), 1000); 
    
    // loops through the list of urls in COLLECTIONS 
    for (curr = urls->first; curr != NULL; curr = curr->next) { 
        temp = strdup(curr->value);
        strcat(temp, ".txt"); 
        page = fopen(temp,"r"); 
         // From section 1 start to end 
        // the while loop breaks when we tell it to 
        
        while (1) { 
        
            int retval = fscanf(page, "%s", dest); 
            
            // if its the end of file , break 
            if (retval == EOF){
                break; 
            } 
            // if you find the work #end, break 
            else if (strcmp(dest, "#end") == 0){
                break; 
              } 
              // if the word url is in urlxx continue 
            else if (strstr(dest, "url")){
                if (!strstr(curr->value, dest)) {
                    addEdge(g, curr->value, dest);
                }
            } 
                
        } 
        
        fclose(page); 
    } 

    return g; 

}

// Create empty graph (use graph ADT in say graph.h and graph.c)
// For each url in the above list
// read <url>.txt file, and update graph by adding a node and outgoing links


// GetInvertedList(List_of_urls)
// Create empty inverted list (use say List of lists, BST where values are lists, etc)
// For each url in List_of_Urls
// read <url>.txt file, and update inverted index
