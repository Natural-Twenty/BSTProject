
#ifndef INVERTEDINDEXBST_H
#define INVERTEDINDEXBST_H


InvertedIndexBST newIIBST (void);

InvertedIndexBST createIIBST (char *word, FileList fileList);

bool wordDupeIsInFile(InvertedIndexBST t, char *word, char *filename);

InvertedIndexBST insertInvertedIndexInOrder (InvertedIndexBST t, char *filename,  char *word,  double tf);

InvertedIndexBST searchBST (InvertedIndexBST tree, char *searchWord);
