//  Implementation of doubly-linked list ADT

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "tfIdfLList.h"

#define TRUE 1
#define FALSE 0

// create a new LListNode
LListNode *newLListNode(char *it)
{
	LListNode *new;
	new = malloc(sizeof(LListNode));
	assert(new != NULL);
	new->count = 0;
	new->value = strdup(it);
	new->prev = new->next = NULL;
	new->NoSearchTerms = 0;
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

// append to linked list
void appendLList(LList L, char *word) {
    
    if (L == NULL) {
        L = newLList();
    }
    
    LListNode *newNode = newLListNode(word);
    LListNode *curr = L->last;
                
    if (L->nitems == 0) {
        L->first = newNode;
        newNode->prev = NULL;
        curr = newNode;
    }
    // If the text buffer has more than 0 Lines
    else {

        curr->next = newNode;
        curr->next->prev = curr;
        curr = curr->next;
    }

    L->last = newNode;
    L->last->next = NULL;
    L->nitems++;
    
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

// swaps any two nodes in a LList
void swapLListNode(LList L, LListNode *prev, LListNode *next) {

    if (L->first != prev) {
        prev->prev->next = next;
    }
        
    if (L->last != next) {
        next->next->prev = prev;
    }

    prev->next = next->next;
    next->prev = prev->prev;
    
    prev->prev = next;
    next->next = prev;
    
    if (L->first == prev) {
        L->first = next;
        
    } else if (L->last == next) {
        L->last = prev;
    }

}

// searches if a value exists within a list
// returns -1, 0, 1 if L is NULL or empty, Value doesn't exist, or exists respectively.
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

// returns the first node in L given the search word
LListNode *getNode(LList L, char *word) {

    LListNode *cur = L->first;
    while (cur != NULL) {
        if (strcmp(word, cur->value) == 0) {
            break;
        }
        cur = cur->next;
    }
    
    return cur;

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

