#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "LList.h"
#include "Graph.h"
#include "pagerank.h"
#include "ReadData.h"
#include "inverted.h"
LList getMatch(LList terms);
void addtoMatchList(LList matches, char *url); 
LList orderMatchList(LList matches);
double getPRfromList (char *url); 
LList pageRankList(LList matches);

void printOrder(LList matches, LList prList, int max);
void getLargest(LList matches, LList prList, int *checked);

int main(int argc, char *argv[]) {
//gcc -Wall -Werror -std=gnu99 -O -lm -o searchPagerank searchPagerank.c LList.c -g
    if (argc <= 1) {
        fprintf(stderr, "Usage: %s <search term> - received less than one arguments\n", argv[0]);
        return 1;
    }

    FILE *data;
    
    if ((data = fopen("pagerankList.txt","r")) == NULL) {
	    fprintf(stderr, "Couldn't open file: pagerankList.txt");
	    return 1;
    }
    fclose(data); 
    
    if ((data = fopen("invertedIndex.txt","r")) == NULL) {
	    fprintf(stderr, "Couldn't open file: invertedIndex.txt");
	    return 1;
    }
    fclose(data);
        
    // Create a linked list for search terms using command line arguments
    LList new = newLList();
    LListNode *curr = NULL;
    int i = 1;
    for (i = 1; i < argc; i++) {
        char *val = strdup(argv[i]);
        LListNode *newNode = newLListNode(val);
        free(val);
        if (new->nitems == 0) {
            new->first = newNode;
            newNode->prev = NULL;
            curr = newNode;
        }
        else {
            curr->next = newNode;
            curr->next->prev = curr;
            curr = curr->next;
        }
        new->last = newNode;
        new->last->next = NULL;
        new->nitems++;
    }

    LList matches = getMatch(new); 
    LList prList = pageRankList(matches);
    int max = matches->nitems; 
    printOrder(matches, prList, max);
    
    freeLList(matches);
    freeLList(prList);
    freeLList(new);
    
 //   LList order = orderMatchList(matches);
    // open invertedIndex.txt, search for words,
    // fscanf the urls for each search term into an array, goes through the linked list to see if the url has already appeared, if it has, increment count, if it hasnt, create a new node 
    
    

  return 0;
}

void printOrder(LList matches, LList prList, int max) {
    int i = 0;
    int *checked = calloc(max, sizeof(int));

    for (i = 0; i < max && i < 30; i++) {
        getLargest(matches, prList, checked);
    }
    free(checked);
}

void getLargest(LList matches, LList prList, int *checked) {
    int max = matches->nitems;
    LListNode *pcurr = prList->first;
    LListNode *mcurr = matches->first;
    double topPR = 0.0000;
    double topMatch = 0.0000;
    int topIndex = 0;
    int i = 0;
    char *topURL = matches->first->value;
    for (i = 0; i < max; i++) {
      
        if ((topMatch < mcurr->count || (topMatch <=mcurr->count && topPR < pcurr->count)) && checked[i] == 0) {
          topMatch = mcurr->count;
          topPR = pcurr->count;
          topURL = mcurr->value;
          topIndex = i;
        }
       
        mcurr = mcurr->next;
        pcurr = pcurr->next;
    }
    checked[topIndex] = 1;
    printf("%s\n", topURL);
}

LList pageRankList(LList matches) {
    LList prList = newLList();
    LListNode *curr = matches->first; 
    LListNode *newCurr = NULL;
    LListNode *newNode = NULL;
    for (curr = matches->first; curr != NULL; curr = curr->next) {
        newNode = newLListNode(curr->value);
        newNode->count = getPRfromList(curr->value);
        if (prList->nitems == 0) {
            prList->first = newNode;
            newNode->prev = NULL;
            newCurr = newNode;
        }
        else {
            newCurr->next = newNode;
            newCurr->next->prev = newCurr;
            newCurr = newCurr->next;
        }
        prList->last = newNode;
        prList->last->next = NULL;
        prList->nitems++;
    }
    return prList;
}

double getPRfromList (char *url) {
    FILE *pagerankList = fopen("pagerankList.txt", "r");
    char *value = malloc(sizeof(char) * 1000);
    int outlinks;
    double pageRank = 0.000; 
    while (fscanf(pagerankList, "%s", value)){
        if (strstr(value, url)) {
            fscanf(pagerankList, "%d, %lf", &outlinks, &pageRank);
            break;
        }

    }
    fclose(pagerankList);
    free(value);
    return pageRank;
}
LList getMatch(LList terms) {
    FILE *index = fopen("invertedIndex.txt", "r");
    
 // dest is the word that you check if its the end of section 
    char *word = calloc(sizeof(char), 1000); 
    LList matches = newLList();
    LListNode *curr = terms->first; 
    int urlscan = 0;
    while (fscanf(index, "%s", word) == 1) { 
       // printf("%s \t", word);
        if (!strstr(word, "url")) {
            urlscan = 0; 
            // Function that searches through the matches linked list for the url and if its already in the linked list, increments count
            for (curr = terms->first; curr != NULL; curr = curr->next) {
                if (strcmp(word, curr->value) == 0) {
                    urlscan = 1;
                    break;
                }
                   
            }
        }
        
        if(strstr(word, "url") && urlscan == 1) {
            addtoMatchList(matches, word);
            
        }

    }
          
    fclose(index);
    free(word);
    return matches;
    // For each seach term, 
}

void addtoMatchList(LList matches, char *url) {
    // If the matches list has 0 items
    LListNode *newNode;
    if (matches->nitems == 0) {
        newNode = newLListNode(url);
        matches->first = matches->last = newNode;
        newNode->prev = matches->last->next = NULL;
        newNode->count = 1;
        matches->nitems++;
    }
    
    else {
        LListNode *curr = matches->first;
        int done = 0;
        for (curr = matches->first; curr != NULL; curr = curr->next) {
            if (strcmp(curr->value, url) == 0) {
                curr->count++;
                done = 1;
                break;
            }
        }
        
        if (done == 0) {
            newNode = newLListNode(url);
            newNode->count = 1;
            matches->last->next = newNode;
            newNode->prev = matches->last;  
            matches->last = newNode; 
            matches->last->next = NULL;
            matches->nitems++;
        }
    }
    
}
