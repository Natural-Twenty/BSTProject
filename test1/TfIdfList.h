
#ifndef TFIDFLIST_H
#define TFIDFLIST_H

#include "invertedIndex.h"

// Creates an empty TfIdfList
TfIdfList newTfIdfList (void);

// Frees a given TfIdfList
void freeTfIdfList(TfIdfList list);

// Creates a TfIdfList node with the given arguments.
TfIdfList createTfIdfNode (char *filename, double tfIdfSum);

// Inserts a given TfIdfList node into the given TfIdfList
TfIdfList insertTfIdfNode (TfIdfList list, TfIdfList insert);

// Checks if file is  duplicate in a TfIdfList
bool fileIsDupe (TfIdfList list, char *filename);

// Swaps the value fields of two TfIdfList nodes.
void swap (TfIdfList list1, TfIdfList list2);

// Sorts a given TfIdfList in descending order according to tfIdfSum.
// If two tfIdfSums are equal, sort according to the filename in alphabetical order.
TfIdfList sortTfIdfList (TfIdfList list);

// Searches a TfIdfList and returns the node with the highest tfIdfSum value.
TfIdfList findTfIdfMax(TfIdfList list);

// Adds the given tfIdfSum to an existing TfIdfList node with the same given filename.
TfIdfList addTfIdfSum (TfIdfList list, char *filename, double tfIdfSum);
#endif
