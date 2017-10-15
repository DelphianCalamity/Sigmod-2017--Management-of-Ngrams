#include <stdio.h>
#include <stdlib.h>
#include "trieStructs.h"
#include <string.h>
#include "errorMessages.h"
#include "ngram.h"


TrieRoot* trieRootInit() {

    TrieRoot *trieroot;

	if ((trieroot = malloc(sizeof(TrieRoot))) == NULL) {
		perror(getError(1));
		exit(1);
	}

	trieroot->root = trieNodeInit(0, NULL, "");

	return trieroot;
}

TrieNode* trieNodeInit(char isFinal, TrieNode *parent, char *word){
	TrieNode *newNode;

	/*Allocate memory for new node*/
	if ((newNode=malloc(sizeof(TrieNode)))==NULL){
		perror(getError(1));
		exit(1);
	}

	/*Initialize basic info of node*/
	newNode->emptySpace = MINSIZE;
	newNode->maxChildren = MINSIZE;
	newNode->deletedChildren = 0;
	newNode->is_final = isFinal;
	newNode->parentNode = parent;

	/*Initialize the word for the new node*/
	if ((newNode->word = malloc(sizeof(strlen(word))))==NULL){
		perror(getError(1));
		exit(1);
	}
	strcpy(newNode->word, word);

	/*Initiliaze new node's children*/
	if ((newNode->children = malloc(MINSIZE*sizeof(TrieNode)))==NULL){
		perror(getError(1));
		exit(1);
	}

	return newNode;
}

void trieAddToChildren(TrieNode *parent, TrieNode *child){
	TrieNode **newChildren;

    /*Run binary search*/

	/*If the parent node has no more space for new children, allocate extra space*/
	if (parent->emptySpace == 0){
		if ((newChildren = realloc(parent->children, (parent->maxChildren*2)*sizeof(TrieNode)))==NULL){
		perror(getError(2));
			exit(2);
		}
		parent->children = newChildren;
        parent->emptySpace += parent->maxChildren;
		parent->emptySpace = parent->maxChildren/2;
	}

	/*Store the new child and update children count*/
	//memcpy(parent->children[parent->maxChildren-parent->emptySpace], child, sizeof(TrieNode));
	parent->emptySpace--;
}


//If found is 1, element exists and 'position' holds its actual position
//Otherwise, 'position' holds the position inside the array where element should be.

BinaryResult trieBinarySearch(TrieNode* parent, char * word){

    TrieNode *children = parent->children;
    
    BinaryResult br;
    br.found = 0;
    
    int size = parent->maxChildren - parent->emptySpace;        //Size of occupied array
    int fst = 0, lst = size-1;
    int middle = (fst + lst)/2;
    
    while(fst <= lst) {
        
        if(strcmp(children[middle].word, word) < 0)
            fst = middle + 1;
     
        else if(strcmp(children[middle].word, word) == 0){
            br.position = middle;
            br.found = 1;
            return br;
        }
        
        else lst = middle - 1;
        
        middle = (fst + lst)/2;
    }
    
    if(fst > size-1)
        br.position = lst;
    else br.position = fst;

    return br;
}





TrieNode* trieSearch(NgramVector *ngramVector){
    
}

int trieInsertSort(NgramVector *ngramVector){
    
    TrieNode *root = trieRoot->root;
    
    
    return 0;
}

void trieDeleteNgram(NgramVector *ngram){

}



void trieFree(){
 
}