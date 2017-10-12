//
// Created by Stathis on 10/11/2017.
//
#include <stdio.h>
#include <stdlib.h>
#include "trieStructs.h"

TrieRoot * rootInit() {
	TrieRoot *trieroot;

	if ((trieroot = malloc(sizeof(TrieRoot))) == NULL) {
//        perror(printError(1));
		exit(1);
	}
	if ((trieroot->root = malloc(sizeof(TrieNode)))==NULL){
//        perror(printError(1));
		exit(1);
	}
}

