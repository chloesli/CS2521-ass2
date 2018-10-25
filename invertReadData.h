// By... Midog, chlodog, Berndog

#ifndef INVERT_READDATA_H
#define INVERT_READDATA_H

#include "LList.h"
#include "Graph.h"
#include "inverted.h"
#include "ReadData.h"
#include "invertReadData.h"

#define TRUE 1
#define FALSE 0

// Create empty inverted list
// For each url in List_of_Urls
// read <url>.txt file, and update inverted index

InvertedList GetInvertedList(LList urls);

#endif
