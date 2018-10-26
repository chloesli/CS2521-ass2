#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "LList.h"
#include "Graph.h"
#include "ReadData.h"
#include "strdup.h"
/************************************************************
  FUNCTION DECLARATIONS
************************************************************/

LList getMatch(LList terms);
void addtoMatchList(LList matches, char *url);
LList orderMatchList(LList matches);
double getPRfromList (char *url);
LList pageRankList(LList matches);

void printOrder(LList matches, LList prList, int max);
void getLargest(LList matches, LList prList, int *checked);

int main(int argc, char *argv[]) {
    // If there are no enough arguments, print out an error message
    if (argc <= 1) {
        fprintf(stderr, "Usage: %s <search term> \n", argv[0]);
        return 1;
    }
    // Check pagerankList.txt is not NULL
    FILE *data;

    if ((data = fopen("pagerankList.txt","r")) == NULL) {
	    fprintf(stderr, "Couldn't open file: pagerankList.txt");
	    return 1;
    }
    fclose(data);
    // Check invertedIndex.txt is not NULL
    if ((data = fopen("invertedIndex.txt","r")) == NULL) {
	    fprintf(stderr, "Couldn't open file: invertedIndex.txt");
	    return 1;
    }
    fclose(data);

    // Create a linked list for search terms using command line arguments
    LList new = newLList();
    LListNode *curr = NULL;
    int i = 1;
    // Loop while index is less than argc
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
    // Get a list of urls that contains the search terms
    LList matches = getMatch(new);
    // Get a list of pageranks for the urls
    LList prList = pageRankList(matches);
    // Prints urls in the correct order
    int max = matches->nitems;
    printOrder(matches, prList, max);

    /************************************************************
     FREE MEMORY ALLOCATED TO LISTS
    ************************************************************/
    freeLList(matches);
    freeLList(prList);
    freeLList(new);

  return 0;
}
// Prints the urls in the correct order
void printOrder(LList matches, LList prList, int max) {
    int i = 0;
    // Checked array
    int *checked = calloc(max, sizeof(int));
    // While i is less than the number of matches and 30
    for (i = 0; i < max && i < 30; i++) {
        // Get the url with the highest number of search terms and PR list that hasnt been checked
        getLargest(matches, prList, checked);
    }
    free(checked);
}

void getLargest(LList matches, LList prList, int *checked) {
    int max = matches->nitems;
    LListNode *pcurr = prList->first;
    LListNode *mcurr = matches->first;
    // Keep track of the top PageRank, Top number of search terms and the index of the topurl
    double topPR = 0.0000;
    double topMatch = 0.0000;
    int topIndex = 0;
    int i = 0;
    // URL name
    char *topURL = matches->first->value;
    for (i = 0; i < max; i++) {
        /************************************************************
        If the current url:
        - Has more search term matches
        - OR has the same amount of search terms and has a larger PageRank than the current topURL
        - And the url has not been printed
        Set it to the TOPURL
        ************************************************************/
        if ((topMatch < mcurr->count || (topMatch <=mcurr->count && topPR < pcurr->count)) && checked[i] == 0) {
          topMatch = mcurr->count;
          topPR = pcurr->count;
          topURL = mcurr->value;
          topIndex = i;
        }
        // Move to the next nodes in the lists
        mcurr = mcurr->next;
        pcurr = pcurr->next;
    }
    // Set the topURL to checked
    checked[topIndex] = 1;
    // Print out the URL
    printf("%s\n", topURL);
}
// Returns a list of PageRanks for the urls in matches
LList pageRankList(LList matches) {
    LList prList = newLList();
    LListNode *curr = matches->first;
    LListNode *newCurr = NULL;
    LListNode *newNode = NULL;
    // Loops through the list of URLS
    for (curr = matches->first; curr != NULL; curr = curr->next) {
        // Makes a node with its url name as its value
        newNode = newLListNode(curr->value);
        // Sets the node's count (a double) to the nodes PageRank
        newNode->count = getPRfromList(curr->value);
        // Adds the node to the end of the list
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
    // Returns a list
    return prList;
}

// Gets a node's PageRank by reading pagerankList.txt
double getPRfromList (char *url) {
    FILE *pagerankList = fopen("pagerankList.txt", "r");
    char *value = malloc(sizeof(char) * 1000);
    int outlinks;
    double pageRank = 0.000;
    // Scans the pagerankList for the url
    while (fscanf(pagerankList, "%s", value)){
        // If the url is found
        if (strstr(value, url)) {
            // Put the next number and double into outlinks and PageRank as we know the format of pagerankList.txt
            fscanf(pagerankList, "%d, %lf", &outlinks, &pageRank);
            break;
        }

    }
    fclose(pagerankList);
    free(value);
    // Return the PageRank of the url
    return pageRank;
}

// Returns a linked list of url matches given the list of search terms
LList getMatch(LList terms) {
    // Reads from invertedIndex.txt
    FILE *index = fopen("invertedIndex.txt", "r");

    char *word = calloc(sizeof(char), 1000);
    // Create a new empty list
    LList matches = newLList();
    LListNode *curr = terms->first;
    // Checks if urls should be taken in
    int urlscan = 0;
    while (fscanf(index, "%s", word) == 1) {
        // If the current string is not a url
        if (!strstr(word, "url")) {
            urlscan = 0;
            // Search through the list of terms to check if the word is a search term
            for (curr = terms->first; curr != NULL; curr = curr->next) {
                // If it is, set urlscan to 1
                if (strcmp(word, curr->value) == 0) {
                    urlscan = 1;
                    break;
                }

            }
        }
        // If the string is a url and urlscan is true
        // The url contains the search term
        if(strstr(word, "url") && urlscan == 1) {
            // The search term is added to the matches list
            addtoMatchList(matches, word);

        }

    }

    fclose(index);
    free(word);
    // Returns matches list
    return matches;
}

// Function is called by the getMatch function and simply adds the url to the match list
// If the URL is already in the match list, it increments count by 1
void addtoMatchList(LList matches, char *url) {
    // If the matches list has 0 items
    LListNode *newNode;
    // If the list has no nodes
    if (matches->nitems == 0) {
        newNode = newLListNode(url);
        matches->first = matches->last = newNode;
        newNode->prev = matches->last->next = NULL;
        newNode->count = 1;
        matches->nitems++;
    }
    // Otherwise
    else {
        LListNode *curr = matches->first;
        int done = 0;
        // Loops through the matches list
        for (curr = matches->first; curr != NULL; curr = curr->next) {
            // If the url is already in the list, incremen the nodes count
            if (strcmp(curr->value, url) == 0) {
                curr->count++;
                // Set done = 1 to show that changes are finished
                done = 1;
                break;
            }
        }
        // If the node is not already in the list, make a new node and add it to the end of the matchList
        if (done == 0) {
            newNode = newLListNode(url);
            // Set its count to 1
            newNode->count = 1;
            matches->last->next = newNode;
            newNode->prev = matches->last;
            matches->last = newNode;
            matches->last->next = NULL;
            matches->nitems++;
        }
    }

}
