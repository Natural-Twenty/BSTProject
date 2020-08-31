
#ifndef INVERTEDINDEXBST_H
#define INVERTEDINDEXBST_H

#include <stdbool.h>
#include "FileList.h"
#include "invertedIndex.h"

// Creates an empty Inverted Index node
InvertedIndexBST newIIBST (void);

// Frees a given Inverted Index BST.
void freeIIBST(InvertedIndexBST t);

// Creates a new Inverted Index node using given arguments.
InvertedIndexBST createIIBST (char *word, FileList fileList);

// Checks whether a word in the same file exists in the Inverted Index BST.
bool wordDupeIsInFile(InvertedIndexBST t, char *word, char *filename);

// Calls createIIBST function with arguments and inserts it into the Inverted Index
// BST in order.
InvertedIndexBST insertInvertedIndexInOrder (InvertedIndexBST t, char *filename,  char *word,  double tf);

// Searches Inverted Index BST for a given word.
InvertedIndexBST searchBST (InvertedIndexBST tree, char *searchWord);

// Prints the Inverted Index BST to an output file.
void printIIBST (InvertedIndexBST t, FILE *output);

#endif
