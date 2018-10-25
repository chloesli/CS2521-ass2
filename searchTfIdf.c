// Built by Burndog, CHloDog, Michdog

#include "tfIdfReadData.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "tfIdfLList.h"

#define TRUE 1
#define FALSE 0
#define MAX_OUT 30


double idf(char *word);
void tfIdfSorting(LList totalUrls);
void printResults(LList totalUrls);
LList getUrlsLList(int argc, char *argv[], LList searchTerms);

int main (int argc, char *argv[]) {
    
    if (argc <= 1) {
        fprintf(stderr, "Not enough arguments\n");
        return 1;
    }
    
    
    // create a LList of all Urls with searchterms
    // for each Url count total number of words and instances of search terms
    // create a LList of search terms
    // if a search term comes up then append it to LList and increment counter
    // calcualte tf of idf and then complete calculation and print results
    
    LList searchTerms = newLList();
    LList totalUrls = getUrlsLList(argc, argv, searchTerms);
    LListNode *cur = totalUrls->first;
    while (cur != NULL) {
    
        // open each Url
        char *dest = strdup(cur->value);
        dest = realloc(dest, strlen(dest)+5);
        strcat(dest, ".txt");
        FILE *f = fopen(dest, "r");
        char *val = calloc(sizeof(char), 1000);
        
        int wordCount = 0;
        LList currSearchTerms = newLList();
        int start = FALSE;
        
        // scanning from the file
        while (fscanf(f, "%s", val) == 1) {
        
            // only considers strings when section-2 is found
            if (start == TRUE) {
            
                if (strcmp(val, "#end") == 0) {
                    break;
                }
                
                wordCount++;
                val = normalise(val);
                
                // when function comes across a seachterm
                if (searchValue(searchTerms, val) == 1) { 
                
                    // either create a new node to currSearchTerms
                    if (searchValue(currSearchTerms, val) < 1) {
                    
                        appendLList(currSearchTerms, val);
                        LListNode *temp = getNode(currSearchTerms, val);
                        temp->count = temp->count + 1;
                        
                    // or just increment the counter for that node/searchTerm
                    } else {
                        
                        LListNode *temp = getNode(currSearchTerms, val);
                        temp->count = temp->count + 1;
                    }
                }
            }
            
            if (strcmp(val, "Section-2") == 0) {
                start = TRUE;
            }
            
        }
        
        //calculate results of each url and store them in totalUrl
        LListNode *curST = currSearchTerms->first;
        double sum = 0;
        
        while (curST != NULL) {
        
            double idfResults = idf(curST->value);
            double tfResults = curST->count/wordCount;
            
            double tfIdfResults = tfResults * idfResults;
            
            sum = sum + tfIdfResults;
            
            curST = curST->next;
            
        }
        
        LListNode *temp = getNode(totalUrls, cur->value);
        temp->count = sum;
        temp->NoSearchTerms = currSearchTerms->nitems;
        
        
        fclose(f);
        free(dest);
        free(val);
        cur = cur->next;
        freeLList(currSearchTerms);
    }
    
    freeLList(searchTerms);
    
    // sort tfIdf results and print them to stdout
    tfIdfSorting(totalUrls);
    printResults(totalUrls);
    
    freeLList(totalUrls);
    return 0;
    
}


// *** Additional Functions ***

LList getUrlsLList(int argc, char *argv[], LList searchTerms) {

    LList totalUrls = newLList();
    int i = 1;
    while (i < argc) {
    
        LList urls = getTermUrls(argv[i]);
        appendLList(searchTerms, argv[i]);
        
        // appends all urls from search term to totalurls
        // doesnt append if url already exists
        LListNode *cur = urls->first;
        
        while (cur != NULL) {
            if (searchValue(totalUrls, cur->value) < 1) {
                appendLList(totalUrls, cur->value);
            }
            cur = cur->next;
        }
    
        i++;
        freeLList(urls);
    }
    
    return totalUrls;
}

// print the tfIdf results from the totalUrls LList
void printResults(LList totalUrls) {

    LListNode *curr = totalUrls->first;
    int count = 0;
    while (curr != NULL && count < MAX_OUT) {
        
        printf("%s %.6f\n", curr->value, curr->count); 
    
        curr = curr->next;    
        count++;
    }
}

// bubble sort without early exit to sort the LList of ifIdf results
void tfIdfSorting(LList totalUrls) {
    
    int length = totalUrls->nitems;
    if (length == 0 || length == 1) {
            // do nothing
    
    } else {   
    
        
        LListNode *prev = totalUrls->first;
        LListNode *next = prev->next;
        
        int i = 0;
        while (i < length) {
            
            prev = totalUrls->first;
            next = prev->next;
            int n = 0;
            while (n < length - i && next != NULL) {
            
                if ((prev->count < next->count) && (prev->NoSearchTerms <= next->NoSearchTerms)) {
                    swapLListNode(totalUrls, prev, next);
                    next = prev->next;
                } else {
                    prev = next;
                    next = next->next;
                }
                
                n++;
            }
            i++;
        }
    }
}



// function using both the total NO. of pages and No. of pages with searched term to find
// the inverse document frequency.
double idf(char *word) {
    
    LList urls = getTermUrls(word);
    
    int numPages = getNumPages();
    int numTermPages = urls->nitems;
    
    free(urls);
    
    return log10((double)numPages/numTermPages);

}


