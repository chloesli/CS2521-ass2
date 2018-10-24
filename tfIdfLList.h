// doubly-linked list ADT

#ifndef LLIST_H
#define LLIST_H

// Implements a doubly-linked list of strings
typedef struct LListRep *LList;

typedef struct LListNode {
	char   *value;  
	float count;
	int NoSearchTerms;
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

// append to linked list
void appendLList(LList L, char *word);

// free up all space associated with list
void freeLList(LList);

// swaps the nodes prev and next;
void swapLListNode(LList L, LListNode *prev, LListNode *next);

// searches if a value exists within a list
// returns -1, 0, 1 if L is NULL or empty, Value doesn't exist, or exists respectively.
int searchValue(LList L, char *search);

LListNode *getNode(LList L, char *word);

// return number of elements in a list
int LListLength(LList);

// is the list empty?
int LListIsEmpty(LList);

#endif
