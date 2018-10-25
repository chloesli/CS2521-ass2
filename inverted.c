// written by the BCM team

#include <stdlib.h> 
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "ReadData.h"

#include "inverted.h"

#define TRUE 1
#define FALSE 0

int main(void) {

    FILE *data;
    
    // gcc -Wall -Werror -std=gnu99 -O -lm -o invertTest invertTest.c ReadData.c Graph.c LList.c inverted.c -g
    
    if ((data = fopen("collection.txt","r")) == NULL) {
	    fprintf(stderr, "Couldn't open file: collection.txt");
	    return 1;
    }
    
    fclose(data); 

    LList urlList = GetCollection();
    
    // creates the invertedIndex.txt
    GetInvertedList(urlList);
    
    //InvertedList newIL GetInvertedList(urlList);
    // showTree(newIL);
    
    return 0;

}


// InvertedList is just a pointer to an invertNode.
InvertedList newInvertedList() {
    
    return NULL;
}

// searches for an invertNode and if found returns the urlList associated with it.
// if a node is not found, searchnode returns NULL.
LList searchNode(InvertedList L, char *word) {
    
    if (L == NULL) {
        return NULL;
    }
    
    if (strcmp(word, L->word) == 0) {
        return L->urlList;
        
    } else if (L->left == NULL && L->right == NULL) {
        return NULL;
    
    } else if (strcmp(word, L->word) < 0) {
            return searchNode(L->left, word);
       
    } else if (strcmp(word, L->word) > 0) {
            return searchNode(L->right, word);
        
    }
    
    return NULL;
    
}

// makes and returns an invertNode
static InvertedList makeNode(char *word, char *url) {

	InvertedList new = malloc(sizeof(invertNode));
	assert(new != NULL);
	new->word = strdup(word);
	new->urlList = newLList();
	new->urlList->first = new->urlList->last = newLListNode(url);
	new->urlList->nitems++;
	new->left = new->right = NULL;
	return new;
}

// words are in alphabetical order in a BSTree ADT
InvertedList addInvertNode(InvertedList L, char *word, char *url) {

    if (L == NULL) {
        L = makeNode(word, url);
    
    } else if (strcmp(word, L->word) == 0) {
        fprintf(stderr, "Error: In addInvertNode > trying to add an already exisitng node\n"); 
        abort();
        
    } else if (strcmp(word, L->word) < 0) {
            L->left = addInvertNode(L->left, word, url);
        
    } else if (strcmp(word, L->word) > 0) {
            L->right = addInvertNode(L->right, word, url);
    }
    
    return L;
}





// code copied from Labs to print tree for testing
/*
static void doShowTree(InvertedList t);
void showTree(InvertedList t)
{
	assert(t != NULL);
	doShowTree(t);
}

// ASCII tree printer
// Courtesy: ponnada
// Via: http://www.openasthra.com/c-tidbits/printing-binary-trees-in-ascii/

// data structures
typedef struct asciinode_struct asciinode;
struct asciinode_struct
{
	asciinode * left, * right;
	//length of the edge from this node to its children
	int edge_length;
	int height;
	int lablen;
	//-1=I am left, 0=I am root, 1=I am right
	int parent_dir;
	//max supported unit32 in dec, 10 digits max
	char label[11];
};

// functions
void print_level(asciinode *node, int x, int level);
void compute_edge_lengths(asciinode *node);
void compute_lprofile(asciinode *node, int x, int y);
void compute_rprofile(asciinode *node, int x, int y);
asciinode *build_ascii_tree(InvertedList t);
void free_ascii_tree(asciinode *node);

#define MAX_HEIGHT 10000
int lprofile[MAX_HEIGHT];
int rprofile[MAX_HEIGHT];
#define INFINITY (1<<20)

#define MIN(X,Y)  (((X) < (Y)) ? (X) : (Y))
#define MAX(X,Y)  (((X) > (Y)) ? (X) : (Y))

int gap = 3;  // gap between left and right nodes

//used for printing next node in the same level, 
//this is the x coordinate of the next char printed
int print_next;    

//prints ascii tree for given Tree structure
static void doShowTree(InvertedList t)
{
	asciinode *proot;
	int xmin, i;
	if (t == NULL) return;
	proot = build_ascii_tree(t);
	compute_edge_lengths(proot);
	for (i = 0; i < proot->height && i < MAX_HEIGHT; i++)
		lprofile[i] = INFINITY;
	compute_lprofile(proot, 0, 0);
	xmin = 0;
	for (i = 0; i < proot->height && i < MAX_HEIGHT; i++)
		xmin = MIN(xmin, lprofile[i]);
	for (i = 0; i < proot->height; i++) {
		print_next = 0;
		print_level(proot, -xmin, i);
		printf("\n");
	}
	if (proot->height >= MAX_HEIGHT) {
		printf("(Tree is taller than %d; may be drawn incorrectly.)\n",
			MAX_HEIGHT);
	}
	free_ascii_tree(proot);
}

//This function prints the given level of the given tree, assuming
//that the node has the given x cordinate.
void print_level(asciinode *node, int x, int level)
{
	int i, isleft;
	if (node == NULL) return;
	isleft = (node->parent_dir == -1);
	if (level == 0)
	{
		for (i=0; i<(x-print_next-((node->lablen-isleft)/2)); i++)
			printf(" ");
		print_next += i;
		printf("%s", node->label);
		print_next += node->lablen;
	}
	else if (node->edge_length >= level)
	{
		if (node->left != NULL)
		{
		  for (i=0; i<(x-print_next-(level)); i++)
		  {
		    printf(" ");
		  }
		  print_next += i;
		  printf("/");
		  print_next++;
		}
		if (node->right != NULL)
		{
		  for (i=0; i<(x-print_next+(level)); i++)
		  {
		    printf(" ");
		  }
		  print_next += i;
		  printf("\\");
		  print_next++;
		}
	}
	else
	{
		print_level(node->left,
			    x-node->edge_length-1,
			    level-node->edge_length-1);
		print_level(node->right,
			    x+node->edge_length+1,
			    level-node->edge_length-1);
	}
}


//This function fills in the edge_length and
//height fields of the specified tree
void compute_edge_lengths(asciinode *node)
{
	int h, hmin, i, delta;
	if (node == NULL) return;
	compute_edge_lengths(node->left);
	compute_edge_lengths(node->right);

	// first fill in the edge_length of node 
	if (node->right == NULL && node->left == NULL)
		node->edge_length = 0;
	else
	{
		if (node->left == NULL)
			hmin = 0;
		else {
			for (i = 0; i < node->left->height && i < MAX_HEIGHT; i++)
				rprofile[i] = -INFINITY;
			compute_rprofile(node->left, 0, 0);
			hmin = node->left->height;
		}
		if (node->right == NULL)
			hmin = 0;
		else {
			for (i=0; i<node->right->height && i < MAX_HEIGHT; i++)
		    		lprofile[i] = INFINITY;
			compute_lprofile(node->right, 0, 0);
			hmin = MIN(node->right->height, hmin);
		}
		delta = 4;
		for (i=0; i<hmin; i++) {
			int w = gap+1+rprofile[i]-lprofile[i];
			delta = (delta > w) ? delta : w;
		}

		//If the node has two children of height 1, then we allow the
		//two leaves to be within 1, instead of 2
		if (((node->left != NULL && node->left->height == 1) ||
		      (node->right != NULL && node->right->height == 1))
				&& delta > 4)
			delta--;
		node->edge_length = ((delta+1)/2) - 1;
	}

	//now fill in the height of node
	h = 1;
	if (node->left != NULL)
		h = MAX(node->left->height + node->edge_length + 1, h);
	if (node->right != NULL)
		h = MAX(node->right->height + node->edge_length + 1, h);
	node->height = h;
}

asciinode *build_ascii_tree_recursive(InvertedList t)
{
	asciinode * node;

	if (t == NULL) return NULL;
	node = malloc(sizeof(asciinode));
	node->left = build_ascii_tree_recursive(t->left);
	node->right = build_ascii_tree_recursive(t->right);
	if (node->left != NULL) node->left->parent_dir = -1;
	if (node->right != NULL) node->right->parent_dir = 1;
	sprintf(node->label, "%s", t->word);
	node->lablen = strlen(node->label);;
	return node;
}


//Copy the tree into the ascii node structre
asciinode *build_ascii_tree(InvertedList t)
{
	asciinode *node;
	if (t == NULL) return NULL;
	node = build_ascii_tree_recursive(t);
	node->parent_dir = 0;
	return node;
}

//Free all the nodes of the given tree
void free_ascii_tree(asciinode *node)
{
	if (node == NULL) return;
	free_ascii_tree(node->left);
	free_ascii_tree(node->right);
	free(node);
}

//The following function fills in the lprofile array for the given tree.
//It assumes that the center of the label of the root of this tree
//is located at a position (x,y).  It assumes that the edge_length
//fields have been computed for this tree.
void compute_lprofile(asciinode *node, int x, int y)
{
	int i, isleft;
	if (node == NULL) return;
	isleft = (node->parent_dir == -1);
	lprofile[y] = MIN(lprofile[y], x-((node->lablen-isleft)/2));
	if (node->left != NULL) {
		for (i=1; i <= node->edge_length && y+i < MAX_HEIGHT; i++)
			lprofile[y+i] = MIN(lprofile[y+i], x-i);
	}
	compute_lprofile(node->left, x-node->edge_length-1, y+node->edge_length+1);
	compute_lprofile(node->right, x+node->edge_length+1, y+node->edge_length+1);
}

void compute_rprofile(asciinode *node, int x, int y)
{
	int i, notleft;
	if (node == NULL) return;
	notleft = (node->parent_dir != -1);
	rprofile[y] = MAX(rprofile[y], x+((node->lablen-notleft)/2));
	if (node->right != NULL) {
		for (i=1; i <= node->edge_length && y+i < MAX_HEIGHT; i++)
	  		rprofile[y+i] = MAX(rprofile[y+i], x+i);
	}
	compute_rprofile(node->left, x-node->edge_length-1, y+node->edge_length+1);
	compute_rprofile(node->right, x+node->edge_length+1, y+node->edge_length+1);
}
*/

