// By... Midog, chlodog, Berndog

#ifndef TFIDF_READDATA_H
#define TFIDF_READDATA_H

#define FALSE 0
#define TRUE 1

#include "tfIdfLList.h"

// count and return number of pages from collection.txt
int getNumPages();

// use invertedIndex.txt to return a LList of the pages that contain the searchterm
LList getTermUrls (char *word);

char *normalise(char *string);

#endif
