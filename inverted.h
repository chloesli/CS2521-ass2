// By team CMB

#ifndef INVERTED_H
#define INVERTED_H

#include "LList.h"
#include "inverted.h"

typedef struct invertNode {

    char *word;
    LList urlList;
    struct invertNode *right;
    struct invertNode *left;


} invertNode;


typedef invertNode *InvertedList;


InvertedList newInvertedList();

// returns NULL if a node is not found, else will return a pointer to a linkedlist
LList searchNode(InvertedList L, char *word);

InvertedList addInvertNode(InvertedList L, char *word, char *url);

void printInvertedIndex(InvertedList L);

void showTree(InvertedList t);

#endif
