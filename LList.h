// doubly-linked list ADT

#ifndef LLIST_H
#define LLIST_H

// Implements a doubly-linked list of strings
typedef struct LListRep *LList;

typedef struct LListNode {
	char   *value;  
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

// check sanity of a LList (for testing)
int validLList(LList);

// return number of elements in a list
int LListLength(LList);

// is the list empty?
int LListIsEmpty(LList);

#endif
