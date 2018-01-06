#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../stack.h"
#include "compactTree.h"
#include "../errorMessages.h"
#include "../BloomFilter/bloomFilter.h"

void trieCompactTree() {

	int i, j;
	Stack stack;
	TrieNode *hashtable = trieRoot->hashtable->Phashtable, *node;

	initStack(&stack);
	for (i = 0; i < trieRoot->hashtable->Buckets; i++) {
		node = hashtable + i;
		for (j = 0; j < node->maxChildren - node->emptySpace; j++) {
			push(&stack, &(node->children[j]));
			trieCompactRoot(&stack);
		}
	}
	deleteStack(&stack);
}

void trieCompactRoot(Stack *stack) {

	char *parentWord;
	short *offsets;
	TrieNode *parent, *child, *tempChildren;
	int i, parentLength, childLength, wordCapacity, oLen, offsetCapacity, numberOfChildren;

	while (notEmpty(stack)) {

		parent = pop(stack);
		numberOfChildren = parent->maxChildren - parent->emptySpace;

		if (numberOfChildren == 1) {
			/*Keep parent's information*/
			parentWord = parent->word;
			parentLength = (int) strlen(parentWord) + 1;
			wordCapacity = parentLength;

			/*Initialize offsets*/
			offsetCapacity = 5;
			offsets = (safemalloc(offsetCapacity * sizeof(short)));

			/*Add parent's information to offsets array*/
			if (parent->is_final)
				offsets[0] = (short) -(parentLength - 1);
			else
				offsets[0] = (short) (parentLength - 1);
			oLen = 1;

			/*Proceed to child and all subsequent solo children */
			while (numberOfChildren == 1) {

				/*Get the parent's only child*/
				child = &parent->children[0];
				childLength = (int) strlen(child->word) + 1;

				/*Allocate, if needed, adequate space for the parent and the child word*/
				while (childLength > (wordCapacity - parentLength)) {
					wordCapacity *= 2;
					parentWord = saferealloc(parentWord, wordCapacity * sizeof(char));
				}

				/*Allocate, if needed, adequate space for the new offset*/
				if (offsetCapacity == parent->offsetsSize) {
					offsetCapacity *= 2;
					offsets = saferealloc(offsets, offsetCapacity * sizeof(short));
				}

				/*Concatenate parent word and child word*/
				memcpy(parentWord + parentLength, child->word, childLength * sizeof(char));
				parentLength += childLength;

				/*Add child's lengths to the offset table*/
				if (child->is_final)
					offsets[parent->offsetsSize] = (short) (-(childLength - 1));
				else
					offsets[parent->offsetsSize] = (short) (childLength - 1);
				oLen++;

				/*Free child's word, it is no longer needed*/
				free(child->word);

				tempChildren = parent->children;

				/*Fix parent with new information*/
				parent->word = parentWord;
				parent->children = child->children;

				numberOfChildren = child->maxChildren - child->emptySpace;
				/*If the the child node, has more than 1 children, or no children at all,
				 * we have to update all the information of the parent*/
				if (numberOfChildren != 1) {
					/*Reduce extra space at the offsets array to what is used*/
					offsets = saferealloc(offsets, oLen * sizeof(short));
					parent->offsets = offsets;
					parent->offsetsSize = oLen;
					parent->is_final = child->is_final;
					parent->maxChildren = child->maxChildren;
					parent->emptySpace = child->emptySpace;
					/*Reduce extra space at the word array to what is used*/
					parent->word = saferealloc(parent->word, parentLength * sizeof(char));
				}
				free(tempChildren);
			}
		}

		/*If node is not a leaf - has more than one children - push them in stack*/
		if (parent->emptySpace != parent->maxChildren) {
			for (i = 0; i < parent->maxChildren - parent->emptySpace; i++)
				push(stack, &(parent->children[i]));
		}
	}
}

/****************************************************************************/

void trieSearch_Static(NgramVector *ngramVector, int id) {

	TrieNode *node;
	int i, capacity;
	char *bloomfilter = safecalloc(BLOOMSIZE, sizeof(char));
	char *buffer = safemalloc(BUFFER_SIZE * sizeof(char));

	queryBuffer.buffer[id] = safemalloc(BUFFER_SIZE * sizeof(char));
	capacity = queryBuffer.capacities[id] = BUFFER_SIZE;
	queryBuffer.sizes[id] = 0;
	buffer[0] = '\0';

	for (i = 0; i < ngramVector->words; i++) {                                                                //For all root's children
		node = Hashtable_lookup_Bucket(trieRoot->hashtable, ngramVector->ngram[i]);
		trieSearch_Ngram_Static(node, i, i, ngramVector, &buffer, &capacity, id, bloomfilter);
		buffer[0] = '\0';
	}

	if (queryBuffer.sizes[id] == 0)
		strcpy(queryBuffer.buffer[id], "-1");

	else queryBuffer.buffer[id][queryBuffer.sizes[id] - 1] = '\0';

	free(bloomfilter);
	free(buffer);
}

void trieSearch_Ngram_Static(TrieNode *node, int round, int i, NgramVector *ngramVector, char **buffer, int *capacity, int id, char *bloomFilter) {

	char *ngram;
	BinaryResult br;
	int j, x, space, len = 0;

	for (; i < ngramVector->words; i++) {
		if (node == NULL)                                                                                       //No more nodes
			return;

		trieBinarySearch(&br, node, ngramVector->ngram[i]);

		if (br.found == 0)                                                                                      //If word not found
			return;

		node = &node->children[br.position];

		if (node->offsets != NULL) {                                                                            //Compacted branch -- more than one words inside this node

			for (j = 0, x = 0; j < node->offsetsSize && i < ngramVector->words; j++, i++) {

				if (strcmp(&node->word[x], ngramVector->ngram[i]) != 0)                                         //If not equal - abort
					return;

				if (node->offsets[j] < 0) {                                                                     //If word is final

					/**************************************/
					for (; round <= i; round++) {
						while ((space = strlen(ngramVector->ngram[round])) >= *capacity - len - 1) {
							*capacity *= 2;
							*buffer = saferealloc(*buffer, *capacity * sizeof(char));                        //Re-allocate space
						}
						memcpy(*buffer + len, ngramVector->ngram[round], space * sizeof(char));
						len += space + 1;
						(*buffer)[len - 1] = ' ';
					}
					(*buffer)[len] = '\0';
					/**************************************/

					ngram = safemalloc(len * sizeof(char));
					memcpy(ngram, *buffer, len);
					ngram[len - 1] = '\0';

					if (!findInBloom(ngram, bloomFilter)) {
						while (len > queryBuffer.capacities[id] - queryBuffer.sizes[id]) {
							queryBuffer.capacities[id] *= 2;
							queryBuffer.buffer[id] = saferealloc(queryBuffer.buffer[id], sizeof(char) * queryBuffer.capacities[id]);
						}

						memcpy(queryBuffer.buffer[id] + queryBuffer.sizes[id], ngram, len);
						queryBuffer.sizes[id] += len;

						queryBuffer.buffer[id][queryBuffer.sizes[id] - 1] = '|';//'\0';

//                        topK_Hashtable_insert(hashtable, ngram, len - 1);
//                        topK_Hashtable_Check_LoadFactor(hashtable, len - 1);
					}
					/*else */free(ngram);
				}
				(node->offsets[j] < 0) ? x += (-node->offsets[j] + 1) : (x += node->offsets[j] + 1);
			}

			if (j > 0) i--;
		} else if (node->is_final) {

			/**************************************/
			for (; round <= i; round++) {
				while ((space = strlen(ngramVector->ngram[round])) >= *capacity - len - 1) {
					*capacity *= 2;
					*buffer = saferealloc(*buffer, *capacity * sizeof(char));                        //Re-allocate space
				}
				memcpy(*buffer + len, ngramVector->ngram[round], space * sizeof(char));
				len += space + 1;
				(*buffer)[len - 1] = ' ';
			}
			(*buffer)[len] = '\0';
			/**************************************/

			ngram = safemalloc(len * sizeof(char));
			memcpy(ngram, *buffer, len);
			ngram[len - 1] = '\0';

			if (!findInBloom(ngram, bloomFilter)) {
				while (len > queryBuffer.capacities[id] - queryBuffer.sizes[id]) {
					queryBuffer.capacities[id] *= 2;
					queryBuffer.buffer[id] = saferealloc(queryBuffer.buffer[id], sizeof(char) * queryBuffer.capacities[id]);
				}

				memcpy(queryBuffer.buffer[id] + queryBuffer.sizes[id], ngram, len);
				queryBuffer.sizes[id] += len;

				queryBuffer.buffer[id][queryBuffer.sizes[id] - 1] = '|';//'\0';

//                        topK_Hashtable_insert(hashtable, ngram, len - 1);
//                        topK_Hashtable_Check_LoadFactor(hashtable, len - 1);
			}
			/*else */ free(ngram);
		}
	}
}
