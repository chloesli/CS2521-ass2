//  Implementation of doubly-linked list ADT

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "LList.h"


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

/*
// check sanity of a LList (for testing)
int validLList(LList L)
{
	if (L == NULL) {
		fprintf(stderr,"LList is null\n");
		return 0;
	}
	if (L->first == NULL) {
		// list is empty; curr and last should be null
		if (L->last != NULL || L->curr != NULL) {
			fprintf(stderr,"Non-null pointers in empty list\n");
			return 0;
		}
	}
	else {
		// list is not empty; curr and last should be non-null
		if (L->last == NULL || L->curr == NULL) {
			fprintf(stderr,"Null pointers in non-empty list\n");
			return 0;
		}
	}
	int count;
	LListNode *curr;
	// check scanning forward through list
	count = 0;
	for (curr = L->first; curr != NULL; curr = curr->next) {
		if (curr->prev != NULL && curr->prev->next != curr) {
			fprintf(stderr, "Invalid forward link into node (%s)\n",curr->value);
			return 0;
		}
		if (curr->next != NULL && curr->next->prev != curr) {
			fprintf(stderr, "Invalid backward link into node (%s)\n",curr->value);
			return 0;
		}
		count++;
	}
	if (count != L->nitems) {
		fprintf(stderr, "Forward count mismatch; counted=%d, nitems=%d\n",
		        count, L->nitems);
		return 0;
	}
	// check scanning backward through list
	count = 0;
	for (curr = L->last; curr != NULL; curr = curr->prev) {
		count++;
	}
	if (count != L->nitems) {
		fprintf(stderr, "Backward count mismatch; counted=%d, nitems=%d\n",
		        count, L->nitems);
		return 0;
	}
	// nothing went wrong => must be ok
	return 1;
} */

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

