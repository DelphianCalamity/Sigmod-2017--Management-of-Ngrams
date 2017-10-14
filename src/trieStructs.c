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
	newNode->maxChildren = 0;
	newNode->deletedChildren = 0;
	newNode->is_final = isFinal;
	newNode->parentNode = parent;

	/*Initialize the word for the new node*/
	if ((newNode->word = malloc(sizeof(strlen(word))))==NULL){
		perror(getError(1));
		exit(1);
	}
	strcpy(newNode->word, word);

	/*Initialize new node's children*/
	if ((newNode->children = malloc(MINSIZE*sizeof(TrieNode*)))==NULL){
		perror(getError(1));
		exit(1);
	}

	return newNode;
}

void trieAddToChildren(TrieNode *parent, TrieNode *child){
	TrieNode **newChildren;

	/*If the parent node has no more space for new children, allocate extra space*/
	if (parent->emptySpace == 0){
		if ((newChildren = realloc(parent->children, (parent->maxChildren*2)*sizeof(TrieNode)))==NULL){
		perror(getError(2));
			exit(2);
		}
		parent->children = newChildren;
		parent->maxChildren = parent->maxChildren*2;
		parent->emptySpace = parent->maxChildren/2;
	}

	/*Store the new child and update children count*/
	memcpy(parent->children[parent->maxChildren-parent->emptySpace], child, sizeof(TrieNode));
	parent->emptySpace--;
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