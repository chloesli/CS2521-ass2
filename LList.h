// doubly-linked list ADT
// Taken from COMP2521 S2 Week 3 Lab
#ifndef LLIST_H
#define LLIST_H

// Implements a doubly-linked list of strings
typedef struct LListRep *LList;

typedef struct LListNode {
	char   *value;
	float    count;
	struct LListNode *prev;
	struct LListNode *next;

} LListNode;

typedef struct LListRep {
	int  nitems;      // count of items in list
	LListNode *first; // first node in list
	LListNode *last;  // last node in list
} LListRep;

// create a new empty LList
LList newLList();

// create a new LListNode
LListNode *newLListNode(char *it);

// free up all space associated with list
void freeLList(LList);

// searches if a value exists within a list
// returns -1, 0, 1 if L is NULL or empty, Value doesn't exist or exists respectively.
int searchValue(LList L, char *search);

// return number of elements in a list
int LListLength(LList);

// is the list empty?
int LListIsEmpty(LList);

#endif
