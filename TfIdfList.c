#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#include "invertedIndex.h"
#include "TfIdfList.h"

#define MAX_FILE 100
/*
struct TfIdfNode {
	char *filename;
	double tfIdfSum; // tfidf sum value
	struct TfIdfNode *next;
};
typedef struct TfIdfNode *TfIdfList;
*/


// Creates an empty TfIdfList
TfIdfList newTfIdfList (void)
{
    return NULL;
}

// Frees a given TfIdfList
void freeTfIdfList(TfIdfList list)
{
    TfIdfList curr, next;
    for (curr = list; curr != NULL; curr = next) {
        next = curr->next;
        free(curr->filename);
        free(curr);
    }
}

// Creates a TfIdfList node with the given arguments.
TfIdfList createTfIdfNode (char *filename, double tfIdfSum)
{
    TfIdfList new = malloc(sizeof(*new));
    if (new == NULL) {return NULL;}
    new->filename = malloc(strlen(filename)+1);
    strcpy(new->filename, filename);
    new->tfIdfSum = tfIdfSum;
    new->next = NULL;
    return new;   
}
 
// Insert TfIdfNode to the back of the linked list.

TfIdfList insertTfIdfNode (TfIdfList list, TfIdfList insert)
{
    // return the TfIdfNode to be inserted when we reach a pointer to NULL.
    if (list == NULL) {
        return insert;
    // if given original list contains the same filename as the insert, add the tfIdfSums to
    // the original list.
    } else if (strcmp(list->filename, insert->filename) == 0) {
        list->tfIdfSum = list->tfIdfSum + insert->tfIdfSum;
        return list;
    // Recursive call to next node in the TfIdfNode
    } else {
        list->next = insertTfIdfNode(list->next, insert);
        return list;
    }
}
// Checks if there another filename that is the same as the given filename in the list.
bool fileIsDupe (TfIdfList list, char *filename)
{
    TfIdfList curr = list;
    // If we reach the end of the list, then filename is not a duplicate.
    if (curr == NULL) {
        return false;
    // If filename already exists in the list, then it is a duplicate.
    } else if (strcmp(list->filename, filename) == 0) {
        return true;
    // Recursive calls to next in list.
    } else {
        return fileIsDupe(list->next, filename);
    }
}

// Swaps the value fields of the TfIdfList nodes
void swap (TfIdfList list1, TfIdfList list2)
{
    double tfIdfTemp;
    char temp[100];
    if (list1->tfIdfSum == list2->tfIdfSum) {
        // tfIdfSums are the same so only need to swap the filenames if needed.
        if (strcmp(list1->filename, list2->filename) > 0) {
            strcpy(temp, list1->filename);
            strcpy(list1->filename, list2->filename);
            strcpy(list2->filename, temp);
        }
    // tfSums are different so swap both data fields.
    } else if (list1->tfIdfSum < list2->tfIdfSum) {
        strcpy(temp, list1->filename);
        tfIdfTemp = list1->tfIdfSum;
        strcpy(list1->filename, list2->filename);
        list1->tfIdfSum = list2->tfIdfSum;
        strcpy(list2->filename, temp);
        list2->tfIdfSum = tfIdfTemp;
    } else {
        return;
    }
}
    
// Sorts a given TfIdfList in descending order according to the tfIdfSum.
// If two tfIdfSums are equal, sort in ascending order according to filename.
TfIdfList sortTfIdfList (TfIdfList list)
{
    if (list == NULL) {return NULL;}
    TfIdfList curr = list;
    TfIdfList max;
    // Find max TfIdfSum in the list and compare to current node's TfIdfSum.
    while (curr != NULL) {
        max = findTfIdfMax(curr);
        // Swap if current node is less than or equal to the max TfIdfSum node.
        if (curr->tfIdfSum <= max->tfIdfSum) {
            swap(curr, max);
        }
        curr = curr->next;
    }
    return list;    
}

// Returns the TfIdfList node with the highest tfIdfSum value.
TfIdfList findTfIdfMax(TfIdfList list)
{
    if (list->next == NULL) {
        return list;
    }
    TfIdfList max = list;
    TfIdfList curr = list->next;
    // Find the node with highest TfIdfSum.
    while (curr != NULL) {
        if (curr->tfIdfSum > max->tfIdfSum) {
            max = curr;
        }
        curr = curr->next;
    }
    return max;
}


