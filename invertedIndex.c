
#include <stdio.h>
#include <ctype.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#include "invertedIndexBST.h"
#include "invertedIndex.h"
#include "FileList.h"
#include "TfIdfList.h"

#define FILE_MAX 100
// Function prototypes
static int countWordTotal (char *filename);

static int countWords (char *filename, char *words);
//

// Counts the total words in a file.
static int countWordTotal (char *filename)
{
    FILE *fp = fopen(filename, "r");
    char buffer[FILE_MAX+1];
    int count = 0;
    while (fscanf(fp, "%s", buffer) != EOF) {
        count++;
    }
    fclose(fp);
    return count;
}

// Counts the number of a given word in a file.
static int countWords (char *filename, char *word)
{
    FILE *fp = fopen(filename, "r");
    int count = 0;
    char buffer[FILE_MAX+1];
    while (fscanf(fp, "%s", buffer) != EOF) {
        if (strcmp(word, normaliseWord(buffer)) == 0) {
            count++;
        }
    }
    fclose(fp);
    return count;
}

char *normaliseWord(char *str)
{
    int i = 0;
    int str_start, str_end;
    //detect leading and trailing spaces
    while (isspace(str[i]) == 1) {
        i++;
    }
    str_start = i;
    i = strlen(str)-1;
    while (isspace(str[i]) == 1) {
        i--;
    }
    str_end = i;
    i = 0;
    // remove leading and trailing spaces
    while (str_start <= str_end) {
        str[i] = str[str_start];
        i++;
        str_start++;
        if (str_start == str_end + 1) {
            str[i] = '\0';
        }
    }
    //detect uppercase and convert to lowercase
    for (i = 0; i < strlen(str); i++) {
        if ((str[i] >= 'A') || (str[i] <= 'Z')) {
            str[i] = tolower(str[i]);
        }
    }
    
    //remove end-of-string punctuation ('.', ',', ';' and '?')
    i = strlen(str) - 1;
    if ((str[i] == '.') || (str[i] == ',') ||
        (str[i] == ';') || (str[i] == '?')) {
        str[i] = '\0';
    }
    return str;   
}

InvertedIndexBST generateInvertedIndex(char *collectionFilename)
{
    // Initialise variables
    
    FILE *collection;
    FILE *textfile;
    char filename[100];
    char scannedWord[100];
    int totalWords, wordFrequency;
    double tf;
    InvertedIndexBST tree = newIIBST();
    collection = fopen(collectionFilename, "r");
    // Read through the collection file and count word total.
    while (fscanf(collection, "%s", filename) != EOF) {
        textfile = fopen(filename, "r");
        totalWords = countWordTotal(filename);   
        // Read through textfile and count word frequency
        while (fscanf(textfile, "%s", scannedWord) != EOF) {     
            //strcpy(word, normaliseWord(word));
            char *word = normaliseWord(scannedWord);
            // Proceed if word is not a duplicate from the same file
            if (!wordDupeIsInFile(tree, word, filename)) {
                wordFrequency = countWords(filename, word);
                tf = (double) wordFrequency/totalWords;
                tree = insertInvertedIndexInOrder(tree, filename, word, tf);
            }
            
        }
        
        fclose(textfile);
    }
    fclose(collection);
    return tree;       
}

void printInvertedIndex (InvertedIndexBST tree)
{
    // Open output file and return if unable to open.
    FILE *output = fopen("invertedIndex.txt", "w");
    if (output == NULL) {return;}
    printIIBST(tree, output);
    // Close output file
    fclose(output);
}

TfIdfList calculateTfIdf (InvertedIndexBST tree, char *searchWord, int D)
{
    TfIdfList list = newTfIdfList();
    TfIdfList insert;
    // Search for the Inverted Index node with the given searchWord.
    InvertedIndexBST targetTree = searchBST(tree, searchWord);
    // Calculate components for TfIdf
    int fileCount = countFiles(targetTree->fileList);
    double tfIdfSum, idf;
    // Get the FileList for the given searchWord
    FileList fileList = targetTree->fileList;
    idf = (double) log10((double)D/fileCount);
    // Use the tf for each file in FileList for the TfIdf calculation
    while (fileList != NULL) {
        tfIdfSum = (double) (fileList->tf * idf);
        insert = createTfIdfNode(fileList->filename, tfIdfSum);
        list = insertTfIdfNode(list, insert);
        fileList = fileList->next;
        
    }
    // return the sorted TfIdfList list.
    return sortTfIdfList(list);
}

TfIdfList retrieve (InvertedIndexBST tree, char *searchWords[], int D)
{
    // Get use the first word to get the starting TfIdfList
    int i = 1;
    TfIdfList list = calculateTfIdf(tree, searchWords[0], D);
    TfIdfList insert, currTfIdf, tempTfIdf;
    // For each word after the first, make a new TfIdfList and insert each node into
    // the main list.
    while (searchWords[i] != NULL) {
        insert = calculateTfIdf(tree, searchWords[i], D);
        currTfIdf = insert;
        while (currTfIdf != NULL) {
            tempTfIdf = currTfIdf;
            currTfIdf = currTfIdf->next;
            tempTfIdf->next = NULL;
            list = insertTfIdfNode(list, tempTfIdf);
        }
        i++;
        
    }
    // Return the sorted TfIdfList.
    return sortTfIdfList(list);
}




