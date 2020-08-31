// Interface for FileList linked list

#ifndef FILELIST_H
#define FILELIST_H

#include <stdbool.h>
#include "invertedIndex.h"

// Creates an empty FileList.
FileList newFileList (void);

// Frees a FileList.
void freeFileList(FileList list);

// Creates a FileList node with arguments.
FileList createFileListNode (char *filename, double tf);

// Checks if file exists in a given FileLst.
bool fileIsInList (FileList list, char *filename);

// Creates a new FileList node and inserts it into the given list.
FileList insertFileInOrder (FileList list, char *filename, double tf);

// Counts the number of files in a given FileList.
int countFiles (FileList list);

#endif
