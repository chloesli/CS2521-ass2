// By...Midog, chlodog, Berndog
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#include "ReadData.h"



// -> Create a set (list) of urls to process by reading data from file collection.txt
LList GetCollection() {

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
        
        free(temp);
        fclose(page); 
    } 
    
    free(dest);
    return g; 

}



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

