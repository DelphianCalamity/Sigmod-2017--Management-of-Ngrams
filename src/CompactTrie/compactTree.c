#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../stack.h"
#include "compactTree.h"
#include "../errorMessages.h"
#include "../BloomFilter/bloomFilter.h"
#include "../Hashtable/Hashtable.h"
#include "../TopK/topK_Hashtable.h"

void trieCompactTree() {

	int i, j, numOfchildren;
	Stack stack;
	TrieNode *hashtable = trieRoot->hashtable->Phashtable, *node;

	initStack(&stack);
	for (i = 0; i < trieRoot->hashtable->Buckets; i++) {
		node = hashtable + i;
		numOfchildren = node->maxChildren - node->emptySpace;
		for (j = 0; j < numOfchildren; j++) {
			if (!node->children[j].deleted) {
				push(&stack, &(node->children[j]));
				trieCompactRoot(&stack);
			}
		}
	}
	deleteStack(&stack);
}

void trieCompactRoot(Stack *stack) {

	char *parentWord;
	short *oBuffer;
	TrieNode *parent, *node, *previous, *prePrevious;
	int i, parentLength, wSpace, wCapacity, oLen, oCapacity, tempSize;

	while (notEmpty(stack)) {

		node = parent = pop(stack);
		tempSize = parent->maxChildren - parent->emptySpace;

		if (tempSize == 1) {

			prePrevious = previous = NULL;

			parentWord = parent->word;                                                            //Initializing word-buffer, offset-buffer variables
			parentLength = strlen(parent->word) + 1;
			wCapacity = parentLength;
			oCapacity = 5;
			oBuffer = safemalloc(oCapacity * sizeof(short));
			if (parent->is_final)
				oBuffer[0] = -(parentLength - 1);
			else oBuffer[0] = parentLength - 1;
			oLen = 1;
		}
		while (tempSize == 1) {                                                                //While parents have only one child

			prePrevious = previous;
			previous = parent->children;
			parent = &parent->children[0];

			wSpace = strlen(parent->word) + 1;
			while (wSpace > wCapacity - parentLength) {
				wCapacity *= 2;
				parentWord = saferealloc(parentWord, wCapacity * sizeof(char));                       //Re-allocate space for word-Buffer
			}
			if (oCapacity == oLen) {
				oCapacity *= 2;
				oBuffer = saferealloc(oBuffer, oCapacity * sizeof(short));                      //Re-allocate space for offset-Buffer
			}

			memcpy(parentWord + parentLength, parent->word, wSpace * sizeof(char));
			parentLength += wSpace;                                                                     //Copy current node's word at parentWord

			wSpace--;
			if (parent->is_final)
				oBuffer[oLen] = -wSpace;
			else oBuffer[oLen] = wSpace;
			oLen++;

			free(parent->word);                                                                 //Free allocated space
			tempSize = parent->maxChildren - parent->emptySpace;
			if (prePrevious != NULL)
				free(prePrevious);
		}

		if (node != parent) {                                                                   //Copy last compacted node's rest info to the branch's first node
			node->word = parentWord;
			node->children = parent->children;
			node->maxChildren = parent->maxChildren;
			node->emptySpace = parent->emptySpace;
			free(previous);
			node->offsets = oBuffer;
			node->offsetsSize = oLen;

			/*** PRINT***/
//			int x;
//			for (i=0,x=0; i<oLen; i++) {
//				printf("%s", &parentWord[x]);
//				//x = oBuffer[i];
//				(oBuffer[i] < 0) ? x+=(-oBuffer[i]+1) : (x+=oBuffer[i]+1);
//			}
//			printf("--- ");
//			for (i=0; i<oLen; i++)
//				printf("%d ",oBuffer[i]);
//			printf("\n");
			/*** PRINT***/
		}

		if (node->emptySpace != node->maxChildren) {                                        //If node is not a leaf - has more than one children - push them in stack
			tempSize = node->maxChildren - node->emptySpace;
			for (i = 0; i < tempSize; i++)
				push(stack, &(node->children[i]));
		}
	}
}

void trieSearch_Static(NgramVector *ngramVector, int id) {

		TrieNode *node;
		int i, capacity;
		char *bloomfilter = safecalloc(BLOOMSIZE, sizeof(char));
		char* buffer = safemalloc(BUFFER_SIZE*sizeof(char));

		queryBuffer.buffer[id] = safemalloc(BUFFER_SIZE*sizeof(char));
		capacity = queryBuffer.capacities[id] = BUFFER_SIZE;
		queryBuffer.sizes[id] = 0;
		buffer[0] = '\0';

		for (i=0; i < ngramVector->words; i++) {                                         						//For all root's children
			node = Hashtable_lookup_Bucket(trieRoot->hashtable, ngramVector->ngram[i]);
			trieSearch_Ngram_Static(node, i, i, ngramVector, &buffer, &capacity, id, bloomfilter);
			buffer[0] = '\0';
		}

		if (queryBuffer.sizes[id] == 0)
			strcpy(queryBuffer.buffer[id], "-1");

		else queryBuffer.buffer[id][queryBuffer.sizes[id]-1] = '\0';

		free(bloomfilter);
		free(buffer);
	}

void trieSearch_Ngram_Static(TrieNode *node, int round, int i, NgramVector *ngramVector, char** buffer, int* capacity, int id, char* bloomFilter) {

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
						while((space=strlen(ngramVector->ngram[round])) >= *capacity - len-1) {
							*capacity *= 2;
							*buffer = saferealloc(*buffer, *capacity*sizeof(char));             	 		//Re-allocate space
						}
						memcpy(*buffer + len, ngramVector->ngram[round], space*sizeof(char));
						len += space+1;
						(*buffer)[len-1] = ' ';
					}
					(*buffer)[len]='\0';
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

						queryBuffer.buffer[id][queryBuffer.sizes[id]-1] = '|';//'\0';

                        topK_Hashtable_insert(hashtable, ngram, len - 1);
//                      topK_Hashtable_Check_LoadFactor(hashtable);
                    }
					else free(ngram);
                }
                (node->offsets[j] < 0) ? x += (-node->offsets[j] + 1) : (x += node->offsets[j] + 1);
            }

            if (j > 0) i--;
        }

    else if (node->is_final) {

			/**************************************/
			for (; round <= i; round++) {
				while((space=strlen(ngramVector->ngram[round])) >= *capacity - len-1) {
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

				topK_Hashtable_insert(hashtable, ngram, len - 1);
//				topK_Hashtable_Check_LoadFactor(hashtable);
            }
			else free(ngram);
        }
    }
}
