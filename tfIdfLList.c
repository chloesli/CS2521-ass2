//  Implementation of doubly-linked list ADT

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "tfIdfLList.h"
#include "strdup.h"
#define TRUE 1
#define FALSE 0

// create a new SLListNode
SLListNode *newSLListNode(char *it)
{
	SLListNode *new;
	new = malloc(sizeof(SLListNode));
	assert(new != NULL);
	new->count = 0;
	new->value = strdup(it);
	new->prev = new->next = NULL;
	new->NoSearchTerms = 0;
	return new;
}

// create a new empty SLList
SLList newSLList()
{
	struct SLListRep *L;

	L = malloc(sizeof (struct SLListRep));
	assert (L != NULL);
	L->nitems = 0;
	L->first = NULL;
	L->last = NULL;
	return L;
}

// append to linked list
void appendSLList(SLList L, char *word) {

    if (L == NULL) {
        L = newSLList();
    }

    SLListNode *newNode = newSLListNode(word);
    SLListNode *curr = L->last;

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
void freeSLList(SLList L)
{
	assert(L != NULL);
	SLListNode *curr, *prev;
	curr = L->first;
	while (curr != NULL) {
		prev = curr;
		curr = curr->next;
		free(prev->value);
		free(prev);
	}
	free(L);
}

// swaps any two nodes in a SLList
void swapSLListNode(SLList L, SLListNode *prev, SLListNode *next) {

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
int searchSLLValue(SLList L, char *search) {
    if (L == NULL || L->nitems == 0) {
        return -1;
    }

    int found = FALSE;
    SLListNode *cur = L->first;

    while (cur != NULL) {
        if (strcmp(cur->value, search) == 0) {
            found = TRUE;
        }
        cur = cur->next;
    }

    return found;
}

// returns the first node in L given the search word
SLListNode *getNode(SLList L, char *word) {

    SLListNode *cur = L->first;
    while (cur != NULL) {
        if (strcmp(word, cur->value) == 0) {
            break;
        }
        cur = cur->next;
    }

    return cur;

}

// return number of elements in a list
int SLListLength(SLList L)
{
	return (L->nitems);
}

// is the list empty?
int SLListIsEmpty(SLList L)
{
	return (L->nitems == 0);
}
