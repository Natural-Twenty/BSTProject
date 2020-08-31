
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
static int countWordTotal (char *filename);

static int countWords (char *filename, char *words);

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

static int countWords (char *filename, char *word)
{
    FILE *fp = fopen(filename, "r");
    int count = 0;
    char buffer[FILE_MAX+1];
    printf("1\n");
    printf("%s\n%s\n", filename, word);
    while (fscanf(fp, "%s", buffer) != EOF) {
        printf("2\n");
        if (strcmp(word, normaliseWord(buffer)) == 0) {
            count++;
        }
    }
    printf("3\n");
    fclose(fp);
    return count;
}


/*
void main(int argc, char *argv[])
{
    char *str[100];
    printf("Enter string: ");
    scanf("%s", str);
    printf("Output string: %s", normaliseWord(str);
}
*/

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
    
    // Open collection file of filenames
    collection = fopen(collectionFilename, "r");
    // Read through the collection file and count word total.
    while (fscanf(collection, "%s", filename) != EOF) {
        textfile = fopen(filename, "r");
        printf("\na\n");
        totalWords = countWordTotal(filename);
        
        // Read through textfile and count word frequency
        while (fscanf(textfile, "%s", scannedWord) != EOF) {
            
            //strcpy(word, normaliseWord(word));
            char *word = normaliseWord(scannedWord);
            printf("\nb\n");
            // Proceed if word is not a duplicate from the same file
            if (!wordDupeIsInFile(tree, word, filename)) {
                printf("b.5\n");
                wordFrequency = countWords(filename, word);
                printf("\nc\n");
                tf = (double) wordFrequency/totalWords;
                printf("d\n");
                
                tree = insertInvertedIndexInOrder(tree, filename, word, tf);
                printf("e\n");
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
    InvertedIndexBST targetTree = searchBST(tree, searchWord);
    int fileCount = countFiles(targetTree->fileList);
    double tfIdfSum, idf;
    FileList fileList = targetTree->fileList;
    idf = (double) log10((double)D/fileCount);
    while (fileList != NULL) {
        tfIdfSum = (double) (fileList->tf * idf);
        insert = createTfIdfNode(fileList->filename, tfIdfSum);
        list = insertTfIdfNode(list, insert);
        fileList = fileList->next;
//        free(insert->filename);
//        free(insert);
        
    }
    
    return sortTfIdfList(list);
}

TfIdfList retrieve (InvertedIndexBST tree, char *searchWords[], int D)
{
/*
    TfIdfList list = newTfIdfList();
    TfIdfList insert;
    double tfIdfSum, idf;
    int fileCount;
    if (tree == NULL) {return NULL;}
    int i = 0;
    while (searchWords[i] != NULL) {
        InvertedIndexBST targetTree = searchBST(tree, searchWords[i]);
        FileList currFileList = targetTree->fileList;
        fileCount = countFiles(currFileList);
        idf = (double) log10((double)D/fileCount);
        while (currFileList != NULL) {
            tfIdfSum = (double) (currFileList->tf * idf);
            insert = createTfIdfNode(currFileList->filename, tfIdfSum);
            list = insertTfIdfNode(list, insert);

            currFileList = currFileList->next;
        }
        i++;
    }
*/
    int i = 1;
    TfIdfList list = calculateTfIdf(tree, searchWords[0], D);
    TfIdfList insert, currTfIdf, tempTfIdf;
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
    
    return sortTfIdfList(list);
}




