#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "ngram.h"
#include "bursts.h"
#include <string.h>
#include <stdbool.h>
#include "trieStructs.h"
#include "errorMessages.h"
#include "BloomFilter/bloomFilter.h"
#include "Hashtable/Hashtable.h"
#include "TopK/topK_Hashtable.h"

/*Initialization of trie root*/
void trieRootInit() {
	trieRoot = safemalloc(sizeof(TrieRoot));
	trieRoot->hashtable = Hashtable_create(800, 0.75);
	trieRoot->current_version = 0;
}

/*Initialization of a new trienode*/
void trieNodeInit(char *word, TrieNode *child) {

	/*Initialize basic info of node*/
	child->word = word;
	child->emptySpace = MINSIZE;
	child->maxChildren = MINSIZE;
	child->deletedChildren = 0;
	child->is_final = 0;
	child->deleted = 0;

	/*Initialize new node's children*/
	child->children = safecalloc(MINSIZE, sizeof(TrieNode));
	child->offsets = NULL;
	child->offsetsSize = 0;

	child->D_version = -1;
}

void trieBinarySearch(BinaryResult *br, TrieNode *parent, char *word) {

	int cmp;
	TrieNode *children = parent->children;

	br->found = 0;

	int size = parent->maxChildren - parent->emptySpace;            //Size of occupied array
	int fst = 0, lst = size - 1;
	int middle = (fst + lst) / 2;

	if (lst < 0) {                                                  //Empty array
		br->position = 0;

		return;
	}
	while (fst <= lst) {

		cmp = strcmp(children[middle].word, word);
		if (cmp < 0)
			fst = middle + 1;
		else if (cmp == 0) {
			br->position = middle;
			br->found = 1;
			return;
		}
		else
			lst = middle - 1;
		middle = (fst + lst) / 2;
	}
	br->position = fst;
}

/****************************************************************************/

void trieSearch(NgramVector *ngramVector, int Q_version, int id) {

	TrieNode *node;
	int i, capacity;
	char *bloomfilter = safecalloc(BLOOMSIZE, sizeof(char));
	char* buffer = safemalloc(BUFFER_SIZE*sizeof(char));

	queryBuffer.buffer[id] = safemalloc(BUFFER_SIZE*sizeof(char));
	capacity = queryBuffer.capacities[id] = BUFFER_SIZE;
	queryBuffer.sizes[id] = 0;
	buffer[0] = '\0';

	for (i=0; i < ngramVector->words; i++) {                                         				//For all root's children
		node = Hashtable_lookup_Bucket(trieRoot->hashtable, ngramVector->ngram[i]);
		trieSearch_Ngram(node, i, i, ngramVector, &buffer, &capacity, id, Q_version, bloomfilter);
		buffer[0] = '\0';
	}

	if (queryBuffer.sizes[id] == 0)
		strcpy(queryBuffer.buffer[id], "-1");

	else queryBuffer.buffer[id][queryBuffer.sizes[id]-1] = '\0';


	free(bloomfilter);
	free(buffer);
}

void trieSearch_Ngram(TrieNode *node, int round, int i, NgramVector *ngramVector, char** buffer, int* capacity, int id, int Q_version, char* bloomFilter) {

	char *ngram;
	BinaryResult br;
	int space, len = 0;

	for (; i < ngramVector->words; i++) {
		if (node == NULL)                                                          					//No more nodes
			return;

		trieBinarySearch(&br, node, ngramVector->ngram[i]);

		node = &node->children[br.position];

		//If word not found or deleted
		if (br.found == 0 || node->deleted == 1) {
			return;
		}

		if (node->is_final && node->A_version <= Q_version && (node->D_version > Q_version || node->D_version < 0)) {

			/**************************************/
			for (; round <= i; round++) {
				while((space=(int)strlen(ngramVector->ngram[round])) >= *capacity - len-1) {
					*capacity *= 2;
					*buffer = saferealloc(*buffer, *capacity*sizeof(char));             	 		//Re-allocate space
				}
				memcpy(*buffer + len, ngramVector->ngram[round], space*sizeof(char));
				len += space+1;
				(*buffer)[len-1] = ' ';
			}
			(*buffer)[len]='\0';

			/**************************************/

			ngram = safemalloc(len*sizeof(char));
			memcpy(ngram, *buffer, len);
			ngram[len-1] = '\0';


			if (!findInBloom(ngram, bloomFilter)) {

				while (len > queryBuffer.capacities[id] - queryBuffer.sizes[id]) {
					queryBuffer.capacities[id] *= 2;
					queryBuffer.buffer[id] = saferealloc(queryBuffer.buffer[id], sizeof(char) * queryBuffer.capacities[id]);
				}

				memcpy(queryBuffer.buffer[id] + queryBuffer.sizes[id], ngram, len);
				queryBuffer.sizes[id] += len;

				queryBuffer.buffer[id][queryBuffer.sizes[id]-1] = '|';//'\0';

				topK_Hashtable_insert(hashtable, ngram, len-1);
				//topK_Hashtable_Check_LoadFactor(hashtable, len-1);
			}
			else{
				free(ngram);
			}
		}
	}
}

/****************************************************************************/

void trieCompactSpace(TrieNode *parent) {

	int j, end;

	for (j=0; j < (parent->maxChildren - parent->emptySpace) && parent->deletedChildren > 0; j++){

		if (parent->children[j].deleted == 1) {
			free(parent->children[j].word);
			end = j + 1;

			while (end < parent->maxChildren - parent->emptySpace && parent->children[end].deleted == 1) {
				free(parent->children[end].word);
				end++;
			}
			memmove(&parent->children[j], &parent->children[end], (parent->maxChildren - parent->emptySpace - end) * sizeof(TrieNode));
			parent->emptySpace += end - j;
			parent->deletedChildren -= end - j;
		}
	}
	memset(&parent->children[parent->maxChildren - parent->emptySpace], 0, parent->emptySpace * sizeof(TrieNode));
}

int trieInsertSort(NgramVector *ngramVector) {

	int i;
	char *word;
	TrieNode *parent;
	BinaryResult result;

	if (ngramVector->words > 0) {											//Insert in Root - hashtable

		parent = Hashtable_insert(trieRoot->hashtable, &result, ngramVector->ngram[0]);

		if (result.found != 1)
			ngramVector->ngram[0] = NULL;

		for (i=1; i<ngramVector->words; i++) {

			word = ngramVector->ngram[i];
			trieMakeSpace(&result, parent, word);
			/*Store the new child and update children count*/
			if (result.found != 1) {											//If found != 1 the node must be inserted - otherwise it is already there

				trieNodeInit(word, &parent->children[result.position]);

				if (result.found == 0)                                      	//If found == 2 the node replaced a deleted node
					parent->emptySpace--;
				ngramVector->ngram[i] = NULL;
			}
			parent = &parent->children[result.position];
		}

		parent->A_version = trieRoot->current_version;
		parent->is_final = 1;
	}

	Hashtable_Check_LoadFactor(trieRoot->hashtable);

	return 0;
}

void trieMakeSpace(BinaryResult* result, TrieNode* parent, char* word) {

	/*Run binary search*/
	trieBinarySearch(result, parent, word);

	if (result->found == 0 || parent->children[result->position].deleted == 1) {

		result->found = 0;

		/*If it's within the borders of the children array*/
		if (result->position < parent->maxChildren) {
			/*If there is already available space*/
			if (parent->children[result->position].deleted == 1) {
				result->found = 2;
				parent->deletedChildren -= 1;
				free(parent->children[result->position].word);
			}
			else {
				/*In between the other children*/
				if (result->position < parent->maxChildren - parent->emptySpace) {

					if (parent->emptySpace == 0) {

						parent->children = saferealloc(parent->children, (parent->maxChildren * 2) * sizeof(TrieNode));
						parent->emptySpace += parent->maxChildren;
						parent->maxChildren *= 2;

						memset(&parent->children[parent->maxChildren - parent->emptySpace], 0, parent->emptySpace * sizeof(TrieNode));
					}
					memmove(&parent->children[result->position + 1], &parent->children[result->position], (parent->maxChildren - parent->emptySpace - result->position) * sizeof(TrieNode));
				}
			}
		}
			/*If ngrams needs to go at the end of the children array, but there is no space*/
		else {
			parent->children = saferealloc(parent->children, (parent->maxChildren * 2) * sizeof(TrieNode));
			parent->emptySpace += parent->maxChildren;
			parent->maxChildren *= 2;

			memset(&parent->children[parent->maxChildren - parent->emptySpace], 0, parent->emptySpace * sizeof(TrieNode));
		}
	}
}

/****************************************************************************/

void trieDeleteNgram(NgramVector *ngram) {
	int i;
	Stack s;
	BinaryResult br;
	TrieNode *node;
	bool stop = false;

	if (ngram->words == 0)
		return;

	initStack(&s);
	node = Hashtable_lookup(&br, trieRoot->hashtable, ngram->ngram[0]);               	 //For root
	if (node == NULL) {
		deleteStack(&s);
		return;
	}
	push(&s, node);
	node = &(node->children[br.position]);

	for (i = 1; i < ngram->words; i++) {												//For the rest of the trieNodes

		trieBinarySearch(&br, node, ngram->ngram[i]);
		if (!br.found || node->children[br.position].deleted) {
			deleteStack(&s);
			return;
		}

		push(&s, node);
		node = &(node->children[br.position]);
	}

	if (!node->is_final) {                                                         		// node not final, specified n-gram not found
		deleteStack(&s);
		return;
	}

	node->is_final = 0;
	if (node->emptySpace + node->deletedChildren < node->maxChildren) {          		// there still are active children
		deleteStack(&s);
		return;
	}

	// no active children, delete 'em all
	for (i = 0; i < node->deletedChildren; i++)                                 		// delete the words of inactive children
		free(node->children[i].word);
	free(node->children);                                                       		// delete the array
	node->deleted = 1;

	node = pop(&s);
	node->deletedChildren++;

	//for the rest of the ngram
	while (notEmpty(&s)) {

		i = node->maxChildren - node->emptySpace - 1;
		while (i >= 0 && !stop) {                                                   	// free all deleted children after the last active one (could be all of them)
			if (node->children[i].deleted) {
				free(node->children[i].word);
				node->children[i].deleted = 0;
				node->deletedChildren--;
				node->emptySpace++;
				i--;
			}
			else {
				stop = true;
			}
		}
		memset(&(node->children[i + 1]), 0, (node->maxChildren - i - 1) * sizeof(TrieNode));

		/*Creating space from deleted nodes*/
		if ((double) node->deletedChildren / (node->maxChildren - node->emptySpace) > FACTOR)
			trieCompactSpace(node);

		if (node->emptySpace + node->deletedChildren < node->maxChildren) {           // there still are active children
			deleteStack(&s);
			return;
		}

		if (node->is_final) {                                                         // end of another ngram, return
			deleteStack(&s);
			return;
		}

		free(node->children);                                                         // delete the array
		node->children = NULL;

		node->deleted = 1;
		node = pop(&s);
		node->deletedChildren++;
	}

	if (node->emptySpace + node->deletedChildren < node->maxChildren) {               // there still are active children
		i = node->maxChildren - node->emptySpace - 1;
		while (node->children[i].deleted) {                                        	  // free all deleted children after the last active one
			free(node->children[i].word);
			node->children[i].deleted = 0;
			node->deletedChildren--;
			node->emptySpace++;
			i--;
		}
	}

	/*Creating space from deleted nodes*/
	if ((double) node->deletedChildren / (node->maxChildren - node->emptySpace) > FACTOR)
		trieCompactSpace(node);

	trieRoot->hashtable->Records--;

	deleteStack(&s);
}


void trieFakeDeleteNgram(NgramVector *ngram) {
	int i;
	BinaryResult br;
	TrieNode *node;

	if (ngram->words == 0)
		return;

	node = Hashtable_lookup(&br, trieRoot->hashtable, ngram->ngram[0]);               	 //For root
	if (node == NULL)
		return;

	node = &(node->children[br.position]);

	for (i = 1; i < ngram->words; i++) {												//For the rest of the trieNodes

		trieBinarySearch(&br, node, ngram->ngram[i]);
		if (!br.found || node->children[br.position].deleted)
			return;
		node = &(node->children[br.position]);
	}

	if (!node->is_final) {                                                         		// node not final, specified n-gram not found
		return;
	}

	node->D_version = trieRoot->current_version;
}


/**********************************************************************/
void trieFree() {

	int i;
	TrieNode *root;
	for (i = 0; i < trieRoot->hashtable->Buckets; i++) {
		root = &trieRoot->hashtable->Phashtable[i];
		trieRecursiveFree(root);
	}

	Hashtable_Destroy(trieRoot->hashtable);
	free(trieRoot);
}

void trieRecursiveFree(TrieNode *node) {

	int i;

	if (!node->deleted) {
		for (i = 0; i < node->maxChildren - node->emptySpace; i++)
			trieRecursiveFree(&node->children[i]);
		if (node->children != NULL)
			free(node->children);
		if(node->offsets != NULL)
			free(node->offsets);
	}

	if (node->word != NULL)
		free(node->word);
}



void trie_buffer_Init() {

	queryBuffer.capacity = CMD_INIT;
	queryBuffer.buffer = safemalloc(CMD_INIT * sizeof(char*));
	queryBuffer.capacities = safemalloc(CMD_INIT * sizeof(int));
	queryBuffer.sizes = safemalloc(CMD_INIT * sizeof(int));
}

void trie_buffer_Destroy() {

	free(burst.jobs);
	free(queryBuffer.buffer);
	free(queryBuffer.capacities);
	free(queryBuffer.sizes);
}