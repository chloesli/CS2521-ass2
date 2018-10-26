// written by the BCM team

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "LList.h"
#include "strdup.h"
#define TRUE 1
#define FALSE 0

double calculateSFR(int ntxt, LList C, LList arrayoflists[], double sot[],  double p[], double n);
void swap(double p[], int i, int j);
void permute(double *array,int i,int length, int ntxt, LList C, LList arrayoflists[], double sizeoftxt[], double *min_sfd, double *W, int *copy);
void printArray(double p[], int length);

//gcc -Wall -Werror -std=gnu99 -O -lm -o scaledFootrule scaledFootrule.c LList.c -g


int main(int argc, char *argv[]) {

    // if not enough arguments
    if (argc <= 1) {
        fprintf(stderr, "Usage: %s <search term> - received less than one arguments\n", argv[0]);
        return 1;
    }

    // the number of .txt files given
    int ntxt = argc - 1;

    FILE *data;

    // stores the word for url
    char line[1000] = {0};

    // says there is a list to put into array of lists
    LList inclusion;

    // create an array of lists
    LList arrayoflists[ntxt];

    // create C
    LList C = newLList();

    // the position of the node in the orignial .txt
    double pos = 1;

    int i;
    int j = 0;

    // array of size of each .txt
    double sizeoftxt[1000] = {0};

    // keep a temp for sizeoftxt array because you reset pos every time
    double postemp = 1;

    // for each .txt
    for (i = 1; i <= ntxt; i++){

        // open the .txt
        data = fopen(argv[i], "r");

        // see if you can open the .txt
        if (data == NULL) {
	        fprintf(stderr, "Couldn't open file: %s", argv[i]);
	        fclose(data);
	        return 1;
        }
        //else printf("opened\n");

        // reset pos when you enter a new .txt
        pos = 1;

        // make a list to put into
        inclusion = newLList();

        // make a list from urls in file
        while (fgets(line, 1000, data) != NULL){

            // stores the url in line and puts it in a node
            LListNode *na = newLListNode(line);
            // puts the count as the position from the original .txt
            na->count = pos;
            // appends it to inclusion
            if (inclusion->nitems == 0){
                inclusion->first = na;
                inclusion->last = na;
            }

            else {
                inclusion->last->next = na;
                na->prev = inclusion->last;
                inclusion->last = na;
            }
            na->next = NULL;
            inclusion->nitems++;
            pos++;
            postemp++;
            //printf("pos = %f\n", pos);
            //printf("n->value = %s\n", n->value);
        }
        pos--;
        while (j < postemp - 1){
            sizeoftxt[j] = pos;
            j++;

        }

        // put the list into the array
        arrayoflists[i-1] = inclusion;

        fclose(data);
    }

    /*****
    get C
    *****/

    // for every list in array
    LListNode *curr;
    for (i = 0; i < ntxt; i++){
        // for every node in list
        for (curr = arrayoflists[i]->first; curr != NULL; curr= curr->next){
            // if its not in C already
            if (searchValue(C, curr->value) == -1 || searchValue(C, curr->value) == 0){
                // put into C
                LListNode *m = newLListNode(curr->value);
                m->count = curr->count;
                // appends it to C
                if (C->first == NULL && C->last == NULL){
                    C->first = m;
                    C->last = m;
                }

                else {
                    C->last->next = m;
                    m->prev = C->last;
                    m->next = NULL;
                    C->last = m;
                }
                C->nitems++;
            }
        }
    }

    double n = C->nitems;
    int size = n;

    double min_sfd = 0;

    // p is an array of 1..n
    double p[size];
    double a = 1;
    for (i = 0; i < n; i++){
        p[i] = a;
        //printf("%f\n", p[i]);
        a++;
    }

    double W = 0;
    i = 0;

    // keep a copy of the array that has the lowest sfd
    int copy[size];
    int c = 0;
    for (c = 0; c < size; c++){
        copy[c] = 0;
    }

    // find the min_sfd
    permute(p, i, size, ntxt, C, arrayoflists, sizeoftxt, &min_sfd, &W, copy);

    // printf the min_sfd
    printf("%f\n", min_sfd);

    // print out the urls that correspond to the values in copy
    // for every value in P
    for (i = 0; i < size; i++){

        // find the url place in C
        curr = C->first;
        for (j = 0; j < p[i] -1; j++){
            curr = curr->next;
        }
        printf("%s", curr->value);
    }



    // free the memory associated with C and arrayoflists
    freeLList(C);
    for (i = 0; i < ntxt; i++){
        freeLList(arrayoflists[i]);
    }

    return 0;
}


void swap(double p[], int i, int j){
    double a = p[i];
    p[i] = p[j];
    p[j] = a;
}

void permute(double p[],int i,int length, int ntxt, LList C, LList arrayoflists[], double sizeoftxt[], double *min_sfd, double *W, int *copy) {

    if (length == i){
        *W = calculateSFR(ntxt, C, arrayoflists, sizeoftxt, p, length);
        //printf("W = %f    ", *W);

        if (*min_sfd == 0){
            *min_sfd = *W;

        }
        if (*W < *min_sfd){
            //printf("W = %f    ", *W);
            *min_sfd = *W;
            //printArray(p,length);
            int a;
            for (a = 0; a < length; a++){
                copy[a] = p[a];
            }
        }
    }
  int j = i;
  for (j = i; j < length; j++) {
     swap(p,i,j);
     permute(p,i+1,length, ntxt, C, arrayoflists, sizeoftxt, min_sfd, W, copy);
     swap(p,i,j);
  }
}

void printArray(double p[], int length){
    int i;
    printf("[");
    for (i = 0; i < length; i++){
        printf("%f ", p[i]);
    }
    printf("]\n");
}


// ///////////////////calculate the scaledFootrule ///////////////////
double calculateSFR(int ntxt, LList C, LList arrayoflists[], double sizeoftxt[],  double p[], double n){

    int i = 0;
    int k = 0;
    int j = 0;
    double sfr = 0;
    double yellow = 0;
    int found = FALSE;

    LListNode *curr;
    LListNode *aolcurr;
    /****************************
    calculates each W sums it up
    ****************************/

    // for each .txt
    for (i = 0; i < ntxt; i++){
        k = 0;
        //printf("i = %d\n\n", i);
        // for each url in C
        for (curr = C->first; curr != NULL; curr = curr->next){
            // go to url in corresponding list
            //printf("curr->value = %s", curr->value);
            found = FALSE;
            for (aolcurr = arrayoflists[i]->first; aolcurr != NULL; aolcurr = aolcurr->next){

                // try to find the url from C in the list

                //printf("aolcurr->value = %s", aolcurr->value);
                //printf("l = %d\n", l);
                if (strcmp(aolcurr->value, curr->value) == 0){
                    found = TRUE;
                    break;
                }
            }
            // if found, calculate W
            if (found){
                yellow = (aolcurr->count)/(sizeoftxt[j]) - p[k]/n;
                if (yellow < 0){
                    yellow = yellow - 2*yellow;
                }
                //printf("aolcurr->count = %f\n",aolcurr->count);
                //printf("sizeoftxt[%d] = %f\n", j, sizeoftxt[j]);
                //printf("p[k] = %f\n", p[k]);
                //printf("n = %f\n", n);
                //printf("yellow = %f\n", yellow);
                sfr += yellow;
                //printf("sfr = %f\n\n", sfr);
                /*if (min_sfd == 0){
                    min_sfd = W;
                }*/
                j++;
            }
            k++;
        }

    }

    return sfr;
}
