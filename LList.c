//  Implementation of doubly-linked list ADT

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "LList.h"

#define TRUE 1
#define FALSE 0


// create a new LListNode
LListNode *newLListNode(char *it)
{
	LListNode *new;
	new = malloc(sizeof(LListNode));
	assert(new != NULL);
	new->value = strdup(it);
	new->prev = new->next = NULL;
	return new;
}

// create a new empty LList
LList newLList()
{
	struct LListRep *L;

	L = malloc(sizeof (struct LListRep));
	assert (L != NULL);
	L->nitems = 0;
	L->first = NULL;
	L->last = NULL;
	return L;
}

// free up all space associated with list
void freeLList(LList L)
{
	assert(L != NULL);
	LListNode *curr, *prev;
	curr = L->first;
	while (curr != NULL) {
		prev = curr;
		curr = curr->next;
		free(prev->value);
		free(prev);
	}
	free(L);
}

int searchValue(LList L, char *search) {
    if (L == NULL || L->nitems == 0) {
        return -1;
    }
    
    int found = FALSE;
    LListNode *cur = L->first;
    
    while (cur != NULL) {
        if (strcmp(cur->value, search) == 0) {
            found = TRUE;
        }
        cur = cur->next;
    }
    
    return found;
}


// return number of elements in a list
int LListLength(LList L)
{
	return (L->nitems);
}

// is the list empty?
int LListIsEmpty(LList L)
{
	return (L->nitems == 0);
}

