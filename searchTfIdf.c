// Built by Burndog, CHloDog, Michdog

#include "tfIdfReadData.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "tfIdfLList.h"
#include "strdup.h"
#define TRUE 1
#define FALSE 0
#define MAX_OUT 30


double idf(char *word);
void tfIdfSorting(SLList totalUrls);
void printResults(SLList totalUrls);
SLList getUrlsSLList(int argc, char *argv[], SLList searchTerms);

int main (int argc, char *argv[]) {

    if (argc <= 1) {
        fprintf(stderr, "Not enough arguments\n");
        return 1;
    }


    // create a SLList of all Urls with searchterms
    // for each Url count total number of words and instances of search terms
    // create a SLList of search terms
    // if a search term comes up then append it to SLList and increment counter
    // calcualte tf of idf and then complete calculation and print results

    SLList searchTerms = newSLList();
    SLList totalUrls = getUrlsSLList(argc, argv, searchTerms);
    SLListNode *cur = totalUrls->first;
    while (cur != NULL) {

        // open each Url
        char *dest = strdup(cur->value);
        dest = realloc(dest, strlen(dest)+5);
        strcat(dest, ".txt");
        FILE *f = fopen(dest, "r");
        char *val = calloc(sizeof(char), 1000);

        int wordCount = 0;
        SLList currSearchTerms = newSLList();
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
                if (searchSLLValue(searchTerms, val) == 1) {

                    // either create a new node to currSearchTerms
                    if (searchSLLValue(currSearchTerms, val) < 1) {

                        appendSLList(currSearchTerms, val);
                        SLListNode *temp = getNode(currSearchTerms, val);
                        temp->count = temp->count + 1;

                    // or just increment the counter for that node/searchTerm
                    } else {

                        SLListNode *temp = getNode(currSearchTerms, val);
                        temp->count = temp->count + 1;
                    }
                }
            }

            if (strcmp(val, "Section-2") == 0) {
                start = TRUE;
            }

        }

        //calculate results of each url and store them in totalUrl
        SLListNode *curST = currSearchTerms->first;
        double sum = 0;

        while (curST != NULL) {

            double idfResults = idf(curST->value);
            double tfResults = curST->count/wordCount;

            double tfIdfResults = tfResults * idfResults;

            sum = sum + tfIdfResults;

            curST = curST->next;

        }

        SLListNode *temp = getNode(totalUrls, cur->value);
        temp->count = sum;
        temp->NoSearchTerms = currSearchTerms->nitems;


        fclose(f);
        free(dest);
        free(val);
        cur = cur->next;
        freeSLList(currSearchTerms);
    }

    freeSLList(searchTerms);

    // sort tfIdf results and print them to stdout
    tfIdfSorting(totalUrls);
    printResults(totalUrls);

    freeSLList(totalUrls);
    return 0;

}


// *** Additional Functions ***

SLList getUrlsSLList(int argc, char *argv[], SLList searchTerms) {

    SLList totalUrls = newSLList();
    int i = 1;
    while (i < argc) {

        SLList urls = getTermUrls(argv[i]);
        appendSLList(searchTerms, argv[i]);

        // appends all urls from search term to totalurls
        // doesnt append if url already exists
        SLListNode *cur = urls->first;

        while (cur != NULL) {
            if (searchSLLValue(totalUrls, cur->value) < 1) {
                appendSLList(totalUrls, cur->value);
            }
            cur = cur->next;
        }

        i++;
        freeSLList(urls);
    }

    return totalUrls;
}

// print the tfIdf results from the totalUrls SLList
void printResults(SLList totalUrls) {

    SLListNode *curr = totalUrls->first;
    int count = 0;
    while (curr != NULL && count < MAX_OUT) {

        printf("%s %.6f\n", curr->value, curr->count);

        curr = curr->next;
        count++;
    }
}

// bubble sort without early exit to sort the SLList of ifIdf results
void tfIdfSorting(SLList totalUrls) {

    int length = totalUrls->nitems;
    if (length == 0 || length == 1) {
            // do nothing

    } else {


        SLListNode *prev = totalUrls->first;
        SLListNode *next = prev->next;

        int i = 0;
        while (i < length) {

            prev = totalUrls->first;
            next = prev->next;
            int n = 0;
            while (n < length - i && next != NULL) {

                if ((prev->count < next->count) && (prev->NoSearchTerms <= next->NoSearchTerms)) {
                    swapSLListNode(totalUrls, prev, next);
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

    SLList urls = getTermUrls(word);

    int numPages = getNumPages();
    int numTermPages = urls->nitems;

    free(urls);

    return log10((double)numPages/numTermPages);

}
