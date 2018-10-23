// by the CBM team

#include <stdio.h>
#include <stdlib.h>
#include "ReadData.h"


int main(void) {

FILE *data;
    
    // gcc -Wall -Werror -std=gnu99 -O -lm -o invertTest invertTest.c ReadData.c Graph.c LList.c inverted.c -g
    
    if ((data = fopen("collection.txt","r")) == NULL) {
	    fprintf(stderr, "Couldn't open file: collection.txt");
	    return 1;
    }
    fclose(data); 

    LList urlList = GetCollection();
    printf("%d\n", urlList->nitems);
    struct LListNode *curr;
    for (curr = urlList->first; curr != NULL; curr=curr->next) {
        printf("%s\n", curr->value);
    }
    
    printf("\n");
    
    InvertedList new = GetInvertedList(urlList);
    showTree(new);
    //outputInvertedIndex(new);
    
    return 0;

}
