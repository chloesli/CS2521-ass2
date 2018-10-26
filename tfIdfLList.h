// doubly-linked list ADT

#ifndef SLList_H
#define SLList_H

// Implements a doubly-linked list of strings
typedef struct SLListRep *SLList;

typedef struct SLListNode {
	char   *value;
	float count;
	int NoSearchTerms;
	struct SLListNode *prev;
	struct SLListNode *next;

} SLListNode;

typedef struct SLListRep {
	int  nitems;      // count of items in list
	SLListNode *first; // first node in list
	SLListNode *last;  // last node in list
} SLListRep;


// create a new empty SLList
SLList newSLList();

// create a new SLListNode
SLListNode *newSLListNode(char *it);

// append to linked list
void appendSLList(SLList L, char *word);

// free up all space associated with list
void freeSLList(SLList);

// swaps the nodes prev and next;
void swapSLListNode(SLList L, SLListNode *prev, SLListNode *next);

// searches if a value exists within a list
// returns -1, 0, 1 if L is NULL or empty, Value doesn't exist, or exists respectively.
int searchSLLValue(SLList L, char *search);

// returns the first node in L given the search word
SLListNode *getNode(SLList L, char *word);

// return number of elements in a list
int SLListLength(SLList);

// is the list empty?
int SLListIsEmpty(SLList);

#endif
