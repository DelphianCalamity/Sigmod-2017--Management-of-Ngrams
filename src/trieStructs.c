#include <stdio.h>
#include <stdlib.h>
#include "trieStructs.h"
#include <string.h>
#include "errorMessages.h"
#include "ngram.h"


/*Initialization of trie root*/
void trieRootInit() {
    if ((trieRoot = malloc(sizeof(TrieRoot))) == NULL) {
        perror(getError(1));
        exit(1);
    }
    if ((trieRoot->root=malloc(sizeof(TrieNode)))==NULL){
        perror(getError(1));
        exit(1);
    }
    trieNodeInit(0, NULL, "", trieRoot->root);
}

/*Initialization of a new trienode*/
void trieNodeInit(char isFinal, TrieNode *parent, char *word, TrieNode *child) {

    /*Initialize basic info of node*/
    child->emptySpace = MINSIZE;
    child->maxChildren = MINSIZE;
    child->deletedChildren = 0;
    child->is_final = isFinal;
    child->parentNode = parent;
    child->deleted = 0;

    free(child->word);
    /*Initialize the word for the new node*/
    if ((child->word = malloc(sizeof(strlen(word)))) == NULL) {
        perror(getError(1));
        exit(1);
    }
    strcpy(child->word, word);

    /*Initiliaze new node's children*/
    if ((child->children = malloc(MINSIZE * sizeof(TrieNode))) == NULL) {
        perror(getError(1));
        exit(1);
    }

}

void trieAddToChildren(TrieNode *parent, char *word) {
    TrieNode *newChildren;
    BinaryResult result;

    /*Run binary search*/
    result = trieBinarySearch(parent, word);
    if (result.found = 0) {
        if (parent->children[result.position - 1].deleted) {
            parent->children[result.position].deleted = 0;
            parent->deletedChildren -= 1;
        } else {
            /*If the parent node has no more space for new children, allocate extra space*/
            if (parent->emptySpace == 0) {
                if ((newChildren = realloc(parent->children, (parent->maxChildren * 2) * sizeof(TrieNode))) == NULL) {
                    perror(getError(2));
                    exit(2);
                }
                parent->children = newChildren;
                parent->emptySpace += parent->maxChildren;
                parent->maxChildren *= 2;
            } else {
                memcpy(&parent->children[result.position + 1], &parent->children[result.position],
                       (parent->maxChildren - parent->emptySpace - result.position) * sizeof(TrieNode));
            }
        }
        /*Store the new child and update children count*/

    }
    return;
}


//If found is 1, element exists and 'position' holds its actual position
//Otherwise, 'position' holds the position inside the array where element should be.

BinaryResult trieBinarySearch(TrieNode *parent, char *word) {

    TrieNode *children = parent->children;

    BinaryResult br;
    br.found = 0;

    int size = parent->maxChildren - parent->emptySpace;        //Size of occupied array
    int fst = 0, lst = size - 1;
    int middle = (fst + lst) / 2;

    if (lst < 0) {                                                //Empty array
        br.position = 0;
        return br;
    }

    while (fst <= lst) {

        if (strcmp(children[middle].word, word) < 0)
            fst = middle + 1;
        else if (strcmp(children[middle].word, word) == 0) {
            br.position = middle;
            br.found = 1;
            return br;
        } else
            lst = middle - 1;

        middle = (fst + lst) / 2;
    }

    if (fst > size - 1)
        br.position = lst;
    else
        br.position = fst;

    return br;
}


TrieNode *trieSearch(NgramVector *ngramVector) {

}

int trieInsertSort(NgramVector *ngramVector) {
    int i, final=0;
    TrieNode *parent;
    TrieNode *newChildren;
    BinaryResult result;
    char *word;

    parent=trieRoot->root;

    for (i = 0; i <ngramVector->words ; i++) {
        word = ngramVector->ngram[i];

        /*Run binary search*/
        result = trieBinarySearch(parent, word);

        if (result.found = 0) {
            if (parent->children[result.position - 1].deleted) {
                parent->children[result.position].deleted = 0;
                parent->deletedChildren -= 1;
            } else {
                /*If the parent node has no more space for new children, allocate extra space*/
                if (parent->emptySpace == 0) {
                    if ((newChildren = realloc(parent->children, (parent->maxChildren * 2) * sizeof(TrieNode))) == NULL) {
                        perror(getError(2));
                        exit(2);
                    }
                    parent->children = newChildren;
                    parent->emptySpace += parent->maxChildren;
                    parent->maxChildren *= 2;
                } else {
                    memcpy(&parent->children[result.position + 1], &parent->children[result.position],
                           (parent->maxChildren - parent->emptySpace - result.position) * sizeof(TrieNode));
                }
            }
            if (i==ngramVector->words)
                final =1;
            /*Store the new child and update children count*/
            trieNodeInit(final, parent, word, &parent->children[result.position]);
        }
        parent = &parent->children[result.position];

    }



    return 0;
}

void trieDeleteNgram(NgramVector *ngram) {

}

void trieFree() {

}