// By...Midog, chlodog, Berndog
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "ReadData.h"
#include "invertReadData.h"

// Normalise functions
// delete a non alphabetical char
static char *deletechar(char *string, int index) {
    int size = strlen(string);
    char *a = malloc(sizeof(char) *(strlen(string)));
    memset(a, '\0', size);
    int i;

    // removing unwanted char at start of string
    if (index == 0) {
        i = index + 1;
        strncpy(a, string + i, size - 1);
    }

    // removing unwanted char at end of string
    else if (index == size - 1) {
        i = index;
 
        strncpy(a, string, i);
    
    }
    
    return a;
}

// calls the deletechar function and ensures all chars are of lower case.
static char *normalise(char *string) {

    char *new;
    char *temp;
    
    if (!isalpha(string[0])) {
        new = deletechar(string, 0);
    }

    else {
        new = strdup(string);
        free(string);
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



static void outputInvertedIndex(InvertedList L);
// Create empty inverted list
// For each url in List_of_Urls
// read <url>.txt file, and update inverted index
InvertedList GetInvertedList(LList urls) {
    
    LListNode *cur = urls->first;
    InvertedList L = newInvertedList();
    char *dest = calloc(sizeof(char), 1000); 
    
    //loop through each url 
    while (cur != NULL) {
        FILE *page;
        char *temp = strdup(cur->value);
        strcat(temp, ".txt"); 
        page = fopen(temp,"r"); 
        int start = FALSE;
        
        
        // search through url
        while (1) {
            int retval = fscanf(page, "%s", dest); 
            
            // if its the end of file , break 
            if (retval == EOF) {
                break; 
            } 
            
            if (start == TRUE) {
            
                if (strcmp(dest, "#end") == 0) {
                    break;
                }
                
                // if words are found make an invertedindex
                else {
                    dest = normalise(dest);
                    LList g = searchNode(L, dest);
                    
                    // for every new word add a node to the InvertedList and add the current url
                    if (g == NULL) {
                        L = addInvertNode(L, dest, cur->value);
                        
                    // for every already exisiting word add the curr url                   
                    } else if (!searchValue(g, cur->value)) {
                        g->last->next = newLListNode(cur->value);
                        g->last->next->prev = g->last;
                        g->last = g->last->next;
                        g->last->next = NULL;
                        g->nitems++;
                    }
                }
            }
            
            if (strcmp(dest, "Section-2") == 0) {
                start = TRUE;
            }
        }
        
        cur = cur->next;
        free(temp);
        fclose(page);
    }
    
    outputInvertedIndex(L);
    return L;
}

static void outputInverted(InvertedList L, FILE *f);
// opens or creates the invertedIndex.txt file for writing
static void outputInvertedIndex(InvertedList L) {
    
    FILE *f;
    f = fopen("invertedIndex.txt", "w+");
    outputInverted(L, f);
    fclose(f);
}

// writes results of invertedList by doing a preorder traversal of the tree
// as tree is ordered by alphabetical order
static void outputInverted(InvertedList L, FILE *f) {
    if (L == NULL) return;
    
    outputInverted(L->left, f);
    
    
    LListNode *cur = L->urlList->first;
    fprintf(f,"%s", L->word);
    
    while (cur != NULL) {
        fprintf(f, " %s", cur->value);
        cur = cur->next;
    }
    fprintf(f, "\n");    
    
    outputInverted(L->right, f);
    
}
