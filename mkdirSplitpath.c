#include "types.h"

extern struct NODE* root;
extern struct NODE* cwd;

//make directory
void mkdir(char pathName[]){

    // TO BE IMPLEMENTED
    //
    // YOUR CODE TO REPLACE THE PRINTF FUNCTION BELOW

    if (!strcmp(pathName, "/") || !strcmp(pathName, "")) {
        printf("MKDIR ERROR: no path provided\n");
        return;
    }

    char baseName[64];
    char dirName[strlen(pathName)];
    struct NODE* currNode = NULL;
    struct NODE* siblingNode = NULL;
    struct NODE* parentNode = splitPath(pathName, baseName, dirName);
    if (parentNode == NULL) {
        return;
    }
    
    currNode = parentNode->childPtr;

    while (currNode != NULL) {
        if (!strcmp(currNode->name, baseName)) {
            printf("MKDIR ERROR: directory %s already exists\n", pathName);
            return;
        } else {
            siblingNode = currNode;
            currNode = currNode->siblingPtr;
        }
    }

    struct NODE* newNode = (struct NODE*)malloc(sizeof(struct NODE));
    strcpy(newNode->name, baseName);
    newNode->fileType = 'D';
    newNode->parentPtr = parentNode;
    newNode->childPtr = NULL;
    newNode->siblingPtr = NULL;
    if (parentNode->childPtr == NULL) {
        parentNode->childPtr = newNode;
    } else {
        siblingNode->siblingPtr = newNode;
    }
    printf("MKDIR SUCCESS: node %s successfully created\n", pathName);

    return;
}

//handles tokenizing and absolute/relative pathing options
struct NODE* splitPath(char* pathName, char* baseName, char* dirName) {

    // TO BE IMPLEMENTED
    // NOTE THAT WITHOUT COMPLETING THIS FUNCTION CORRECTLY
    // rm, rmdir, ls, cd, touch COMMANDS WILL NOT EXECUTE CORRECTLY
    // SEE THE PROVIDED SOLUTION EXECUTABLE TO SEE THEIR EXPECTED BEHAVIOR

    // YOUR CODE HERE
    //

    int dirLen = strlen(pathName);
    char pathCopy[dirLen + 1]; // make a string of same length (+ 1 because dirLen doesn't include the null char)
    strcpy(pathCopy, pathName);

    char* storeDelim = strtok(pathCopy, "/");
    char* lastSlash = NULL;

    if (!strcmp(pathName, "/") || !strcmp(pathName, "")) {
        strcpy(dirName, "/");
        strcpy(baseName, "");
        return root;
    }

    while (storeDelim != NULL) {
        lastSlash = storeDelim;
        storeDelim = strtok(NULL, "/");
    }

    strcpy(baseName, lastSlash);

    if (!strcmp(pathName, baseName)) {
        strcpy(dirName, "");
    } else if (!strcmp(pathName + 1, baseName)) {
        strcpy(dirName, "/");
    } else {
        dirLen = strlen(pathName) - strlen(baseName) - 1;
        strncpy(dirName, pathName, dirLen);
        dirName[dirLen] = '\0';
    }

    struct NODE* currNode = NULL;
    strcpy(pathCopy, dirName);
    char* nodeName = strtok(pathCopy, "/");
    if (pathName[0] == '/') {
        currNode = root;
    } else {
        currNode = cwd;
    }


    if (nodeName != NULL) {
        currNode = currNode->childPtr;
    }
    while (nodeName != NULL) {
        if (currNode == NULL) {
            printf("ERROR: directory %s does not exist\n", nodeName);
            return NULL;
        } else if (strcmp(currNode->name, nodeName)) {
            currNode = currNode->siblingPtr;
        } else {
            nodeName = strtok(NULL, "/");
            if (nodeName != NULL) {
                currNode = currNode->childPtr;
            }
        }
    }

    return currNode;
}
