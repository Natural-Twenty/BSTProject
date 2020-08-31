#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#include "invertedIndex.h"
#include "invertedIndexBST.h"
#include "FileList.h"

#define MAX_FILE 100
/*
struct InvertedIndexNode {
	char *word; // key
	struct FileListNode *fileList;
	struct InvertedIndexNode *left;
	struct InvertedIndexNode *right;

};
typedef struct InvertedIndexNode *InvertedIndexBST;
*/

// Creates an empty Inverted Index BST node.
InvertedIndexBST newIIBST(void)
{
    return NULL;
}

// Frees the Inverted Index BST.
void freeIIBST(InvertedIndexBST t)
{
    if (t == NULL) {return;}
    freeIIBST(t->right);
    freeIIBST(t->left);
    free(t->word);
    freeFileList(t->fileList);
    free(t);
}

// Creates a new Inverted Index node with the arguments.
InvertedIndexBST createIIBST (char *word, FileList fileList)
{
    InvertedIndexBST new = malloc(sizeof(*new));
    if (new == NULL) {return NULL;}
    new->word = malloc(strlen(word)+1);
    strcpy(new->word, word);
    new->fileList = fileList;
    new->left = NULL;
    new->right = NULL;
    return new;
}

/* 
    Searches BST for the given file and word. If there is already a word under the
    same file, then the given word under the given file is a duplicate; return true.
    Otherwise, no duplicate found so return false.
*/
bool wordDupeIsInFile(InvertedIndexBST t, char *word, char *filename)
{
    // If no word is found, then there are no duplicates.
    if (t == NULL) {
        return false;
    // If a word is found, check if word is from same file.
    } else if (strcmp(t->word, word) == 0) {
        return fileIsInList(t->fileList, filename);
    // Recursive call to sub-trees depending on alphabetical order
    } else if (strcmp(t->word, word) > 0) {
        return wordDupeIsInFile(t->left, word, filename);
    } else {
        return wordDupeIsInFile(t->right, word, filename);
    }
}

/*
    Constructs a new InvertedIndexBST node and recursively searches the given BST for the given word.
    If a word is found, insert the FileList.
    If no word is found, insert BST node in order at the bottom of the BST.
    
*/
InvertedIndexBST insertInvertedIndexInOrder (InvertedIndexBST t, char *filename,  char *word,  double tf)
{
    // Base case: insert BST node if given tree is empty (given word does no exist in BST
    if (t == NULL) {
        FileList newList = createFileListNode(filename, tf);
        if (newList == NULL) {
            printf("Failed to create new FileList");
        }
        InvertedIndexBST newNode = createIIBST (word, newList);
        if (newNode == NULL) {
            printf("Failed to create new Inverted Index Node");
        }
        return newNode;
    // Base case: word exists in BST so just add the filename to the FileList
    } else if (strcmp(t->word, word) == 0) {
        t->fileList = insertFileInOrder(t->fileList, filename, tf);
        return t;
    // Recursive call to sub-trees
    } else if (strcmp(t->word, word) > 0) {
        t->left = insertInvertedIndexInOrder(t->left, filename, word, tf);
    } else {
        t->right = insertInvertedIndexInOrder(t->right, filename, word, tf);
    }
    return t;
}

// Searches a given Inverted Index BST for a given word.
InvertedIndexBST searchBST (InvertedIndexBST tree, char *searchWord)
{
    if (tree == NULL) {return NULL;}
    InvertedIndexBST curr = tree;
    // Return the node if word is found within BST.
    if (strcmp(curr->word, searchWord) == 0) {
        return curr;
    // Recursive call to subtrees depending on alplabetical order
    } else if (strcmp(curr->word, searchWord) > 0) {
        return searchBST(curr->left, searchWord);
    } else {
        return searchBST(curr->right, searchWord);
    }
}

// Recursive printing function to a given output file. Printing is in-order.
void printIIBST (InvertedIndexBST tree, FILE *output)
{
    // Base case: return to given tree is empty.
    if (tree == NULL) {return;}
    // Recursive call to left tree for in order printing
    printIIBST(tree->left, output);
    
    // Printing function output to file
    fprintf(output, "%s ", tree->word);
    FileList curr = tree->fileList;
    while (curr != NULL) {
        if (curr->next == NULL) {
            fprintf(output, "%s\n", curr->filename);
        } else {
            fprintf(output, "%s ", curr->filename);
        }
        curr = curr->next;
    }
    //Recursive call to right tree for in order printing
    printIIBST(tree->right, output);
}
