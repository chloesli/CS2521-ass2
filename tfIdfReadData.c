// By...Midog, chlodog, Berndog
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#include "tfIdfReadData.h"

// gcc -Wall -Werror -std=gnu99 -O -lm -o searchTfIdf searchTfIdf.c tfIdfReadData.c tfIdfLList.c -g


// functions to calculate Idf of tf-idf
// count and return number of pages from collection.txt
int getNumPages() {

    FILE *f = fopen("collection.txt", "r");
    int count = 0;
    char val[100];
    
    
    while (fscanf(f, "%s", val) == 1) {
        
        if (strstr(val, "url")) { 
            count++;
        }
        
    }
    
    fclose(f);
    return count; 
}

// use invertedIndex.txt to return a LList of the pages that contain the searchterm
LList getTermUrls (char *word) {

    FILE *f = fopen("invertedIndex.txt", "r");
    char val[1000];
    LList new = newLList();
    
    while (fscanf(f, "%s", val) == 1) {
        
        if (strcmp(val, word) == 0) {
        
            while (fscanf(f, "%s", val) == 1) {
                
                if (strstr(val, "url")) { 
                
                   appendLList(new, val);
                    
                } else {
                    break;
                }
            }
        }
    }
    
    fclose(f);
    return new; 

}


// Normalise functions

static char *deletechar(char *string, int index) {
    int size = strlen(string);
    char *a = malloc(sizeof(char) *(strlen(string)));
    memset(a, '\0', size);
    int i;

    if (index == 0) {
        i = index + 1;
        strncpy(a, string + i, size - 1);
    }

    else if (index == size - 1) {
        i = index;
 
        strncpy(a, string, i);
    
    }
    
    return a;
}

char *normalise(char *string) {
    char *new;
    char *temp;
        if (!isalpha(string[0])) {
            new = deletechar(string, 0);
        }

        else {
            new = strdup(string);
            // free(string);
        }
        
        int i = strlen(new) - 1;
        if (!isalpha(new[i])) {
            temp = new;
            new = deletechar(new, i);
            free(temp);
        }
    int x = 0;
    while (new[x]) {
    
            new[x] = tolower(new[x]);
        
        x++;
    }  


    return new;
}



