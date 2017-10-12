//
// Created by Stathis on 10/11/2017.
//
#include <stdio.h>
#include <stdlib.h>
#include "trieStructs.h"
#include <string.h>
#include "errorMessages.h"

TrieRoot * rootInit() {
	TrieRoot *trieroot;

	if ((trieroot = malloc(sizeof(TrieRoot))) == NULL) {
        perror(getError(1));
		exit(1);
	}

    trieroot->root = trieNodeInit(0, NULL, "");

    return trieroot;
}

TrieNode * trieNodeInit(char final, TrieNode *parent, char *word){
    TrieNode *newNode;

    /*Allocate memory for new node*/
    if ((newNode=malloc(sizeof(TrieNode)))==NULL){
        perror(getError(1));
        exit(1);
    }

    /*Initilize basic info of node*/
    newNode->currentChildren = 0;
    newNode->maxChildren = 0;
    newNode->is_final = final;
    newNode->parentNode = parent;

    /*Initiliaze the word for the new node*/
    if ((newNode->word = malloc(sizeof(strlen(word))))==NULL){
        perror(getError(1));
        exit(1);
    }
    strcpy(newNode->word, word);

    /*Initiliaze new node's children*/
    if ((newNode->children = malloc(sizeof(TrieNode *)))==NULL){
        perror(getError(1));
        exit(1);
    }

    return newNode;
}

void addToChildren(TrieNode *parent, TrieNode *child){
    TrieNode **newChildren;

    /*If the parent node has no more space for new children, allocate extra space*/
    if (parent->currentChildren == parent->maxChildren){
        if ((newChildren = realloc(parent->children, (parent->maxChildren*2)*sizeof(TrieNode *)))==NULL){
        perror(getError(2));
            exit(2);
        }
        parent->children = newChildren;
        parent->maxChildren = parent->maxChildren*2;
    }

    /*Store the new child and update children count*/
    parent->children[parent->currentChildren] = child;
    parent->currentChildren++;
}