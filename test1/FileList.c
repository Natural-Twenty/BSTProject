#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#include "invertedIndex.h"
#include "FileList.h"

#define FILE_MAX 100
/*
struct FileListNode {
	char *filename;
	double tf; // relative tf
	struct FileListNode *next;
};
typedef struct FileListNode *FileList;
*/

// Creates an empty FileList
FileList newFileList (void)
{
    return NULL;
}
// Frees a FileList
void freeFileList(FileList list)
{
    FileList curr, next;
    for (curr = list; curr != NULL; curr = next) {
        next = curr->next;
        free(curr->filename);
        free(curr);
    }
}

// Creates a new FileList node with arguments.
FileList createFileListNode (char *filename, double tf)
{
    FileList new = malloc(sizeof(*new));
    if (new == NULL) {return NULL;}
    new->filename = malloc(strlen(filename)+1);
    strcpy(new->filename, filename);
//    new->filename = strdup(filename);
    new->tf = tf;
    new->next = NULL;
    return new;
}

/*
    Searches a given FileList for a given filename.
    If no file is found, then there is no duplicate; return false.
    Otherwise, a file is found so there is a duplicate; return true.
*/
bool fileIsInList (FileList list, char *filename)
{
    FileList curr = list;
    while (curr != NULL) {
        if (strcmp(curr->filename, filename) == 0) {
            return true;
        }
        curr = curr->next;
    }
    return false;
}

/*
    Creates a new FileList node with the filename and relative frequency.
    Searches through the given FileList to insert the new node in 
    alphabetical order.
*/
FileList insertFileInOrder (FileList list, char *filename, double tf)
{
    if ((list == NULL) || (strcmp(list->filename, filename) > 0)) {
        FileList newNode = createFileListNode(filename, tf);
        newNode->next = list;
        return newNode;
    }
    list->next = insertFileInOrder(list->next, filename, tf);
    return list;
}
// Counts number of files in a FileList.
int countFiles (FileList list)
{
    int count = 0;
    FileList curr = list;
    while (curr != NULL) {
        count++;
        curr = curr->next;
    }
    return count;
}


