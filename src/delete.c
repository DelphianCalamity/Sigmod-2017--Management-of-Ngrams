#include <stdio.h>
#include <stdlib.h>
#include "trieStructs.h"
#include <string.h>
#include "errorMessages.h"
#include "ngram.h"


void trieDeleteNgram(NgramVector *ngram) {
	int i;
	BinaryResult br;
	TrieNode *node = trieRoot->root;
	for (i=0; i<ngram->words; i++){
		trieBinarySearch(&br, node, ngram->ngram[i]);
		if (!br.found || node->children[br.position].deleted)
			return;

		node = &(node->children[br.position]);
	}

	if (!node->is_final)
		return;

	node->is_final = 0;

	if (node->emptySpace + node->deletedChildren < node->maxChildren)			// there still are active children
		return;

	// no active children, delete 'em all
	for (i=0; i<node->deletedChildren; i++)									// delete the words of inactive children
		free(node->children[i].word);
	free(node->children);													// delete the array
	node->deleted = 1;
	node = node->parentNode;


	//for the rest of the ngram
	while(node != trieRoot->root){
		if (node->is_final)					// end of another ngram, return
			return;

		if (node->emptySpace + node->deletedChildren < node->maxChildren)			// there still are active children
			return;

		// no active children, delete 'em all
		for (i=0; i<node->deletedChildren; i++)									// delete the words of inactive children
			free(node->children[i].word);
		free(node->children);													// delete the array
		node->deleted = 1;
		node = node->parentNode;
	}
}