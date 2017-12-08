#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../stack.h"
#include "compactTree.h"
#include "../trieStructs.h"
#include "../errorMessages.h"
#include "../BloomFilter/bloomFilter.h"
#include "../Hashtable/Hashtable.h"
#include "../TopK/topK_Hashtable.h"


void trieCompactTree() {

	int i, j;
	Stack stack;
	TrieNode *hashtable = trieRoot->hashtable->Phashtable, *node;

	initStack(&stack);
	for (i = 0; i < trieRoot->hashtable->Buckets; i++) {
		node = hashtable+i;
		for (j = 0; j < node->maxChildren - node->emptySpace; j++) {
			push(&stack, &(node->children[j]));
			trieCompactRoot(&stack);
		}
	}
	deleteStack(&stack);
}

void trieCompactRoot(Stack *stack) {

	char *wBuffer;
	short *oBuffer;
	TrieNode *parent, *node, *previous, *prePrevious;
	int i, wLen, wSpace, wCapacity, oLen, oCapacity, tempSize;

	while (notEmpty(stack)) {

		node = parent = pop(stack);
		tempSize = parent->maxChildren-parent->emptySpace;

		if (tempSize == 1) {
			prePrevious = previous = NULL;

			wBuffer = parent->word;                                                          //Initializing word-buffer, offset-buffer variables
			wLen = strlen(parent->word)+1;
			wCapacity = wLen;
			oCapacity = 5;
			oBuffer = safemalloc(oCapacity * sizeof(short));
			if(parent->is_final)
				oBuffer[0] = -(wLen-1);
			else oBuffer[0] = wLen-1;
			oLen = 1;
		}
		while (tempSize == 1) {																//While parents have only one child

			prePrevious = previous;
			previous = parent->children;
			parent = &parent->children[0];

			wSpace = strlen(parent->word)+1;
			while (wSpace > wCapacity-wLen) {
				wCapacity *= 2;
				wBuffer = saferealloc(wBuffer, wCapacity * sizeof(char));           		//Re-allocate space for word-Buffer
			}
			if (oCapacity == oLen) {
				oCapacity *= 2;
				oBuffer = saferealloc(oBuffer, oCapacity * sizeof(short));          		//Re-allocate space for offset-Buffer
			}

			memcpy(wBuffer + wLen, parent->word, wSpace*sizeof(char));  wLen += wSpace;		//Copy current node's word at wBuffer

			wSpace--;
			if (parent->is_final)
				oBuffer[oLen] = -wSpace;
			else oBuffer[oLen] = wSpace;
			oLen++;

			free(parent->word);																//Free allocated space
			tempSize = parent->maxChildren-parent->emptySpace;
			if (prePrevious != NULL)
				free(prePrevious);
		}

		if (node != parent) {																//Copy last compacted node's rest info to the branch's first node
			node->word = wBuffer;
			node->children = parent->children;
			node->maxChildren = parent->maxChildren;
			node->emptySpace = parent->emptySpace;
			free(previous);
			node->offsets = oBuffer;
			node->offsetsSize = oLen;

			/*** PRINT***/
//			int x;
//			for (i=0,x=0; i<oLen; i++) {
//				printf("%s", &wBuffer[x]);
//				//x = oBuffer[i];
//				(oBuffer[i] < 0) ? x+=(-oBuffer[i]+1) : (x+=oBuffer[i]+1);
//			}
//			printf("--- ");
//			for (i=0; i<oLen; i++)
//				printf("%d ",oBuffer[i]);
//			printf("\n");
			/*** PRINT***/
		}

		if (node->emptySpace != node->maxChildren) {										//If node is not a leaf - has more than one children - push them in stack
			for(i = 0; i <  node->maxChildren-node->emptySpace; i++)
				push(stack, &(node->children[i]));
		}
	}
}
/****************************************************************************/

void trieSearch_Static(NgramVector *ngramVector) {

	int i;
	int check = 0;
	TrieNode *node;
	char* buffer = safemalloc(BUFFER_SIZE*sizeof(char));
	int capacity = BUFFER_SIZE;
	buffer[0] = '\0';

	memset(bloomfilter, 0, BLOOMSIZE);

	for (i=0; i < ngramVector->words; i++) {                                         			//For all root's children
		node = Hashtable_lookup_Bucket(trieRoot->hashtable, ngramVector->ngram[i]);
		trieSearch_Ngram_Static(node, i, i, ngramVector, &buffer, &capacity, &check);
		buffer[0] = '\0';
	}

	if (check > 0) {
#ifndef BLOOM
		trieRoot->lastQuery++;
#endif
	}

	else printf("-1");
	printf("\n");

	free(buffer);
}

void trieSearch_Ngram_Static(TrieNode *node, int round, int i, NgramVector *ngramVector, char** buffer, int* capacity, int *check) {

	char *ngram;
	BinaryResult br;
	int j, x, space, len = 0;

	for (; i < ngramVector->words; i++) {
		if (node == NULL)                                                          							//No more nodes
			return;

		trieBinarySearch(&br, node, ngramVector->ngram[i]);

		if (br.found == 0)              																	//If word not found
			return;

		node = &node->children[br.position];

		if (node->offsets != NULL) {																		//Compacted branch -- more than one words inside this node

			for (j=0,x=0; j < node->offsetsSize && i < ngramVector->words; j++, i++) {

				if (strcmp(&node->word[x], ngramVector->ngram[i]) != 0)                                    	//If not equal - abort
					return;

				if (node->offsets[j] < 0) {                                                                	//If word is final

					for (; round <= i; round++) {
						if ((space = (int) strlen(ngramVector->ngram[round])) >= *capacity - len - 1) {
							*buffer = saferealloc(*buffer, 2 * (*capacity) * sizeof(char));                	//Re-allocate space
							*capacity *= 2;
						}
						memcpy(*buffer + len, ngramVector->ngram[round], space * sizeof(char));
						len += space + 1;
						(*buffer)[len - 1] = ' ';
					}
					(*buffer)[len] = '\0';

					ngram = safemalloc(len * sizeof(char));
					memcpy(ngram, *buffer, len);
					ngram[len - 1] = '\0';

					if (!findInBloom(ngram)) {
						if (*check > 0)
							printf("|");
						printf("%s", ngram);

						(*check)++;

						topK_Hashtable_insert(hashtable, ngram, len-1);
						topK_Hashtable_Check_LoadFactor(hashtable, len-1);
					}
				}
				(node->offsets[j] < 0) ? x+=(-node->offsets[j]+1) : (x+=node->offsets[j]+1);
			}

			if(j > 0)	i--;
		}

#ifndef BLOOM	//WITHOUT BLOOM FILTER
		else if (node->is_final && node->visited < trieRoot->lastQuery) {      					//An ngram is found and is not already 'printed'

			node->visited = trieRoot->lastQuery;

			for (; round <= i; round++) {
				if((space=(int)strlen(ngramVector->ngram[round])) >= *capacity - len-1) {
					*buffer = saferealloc(*buffer, 2 * (*capacity)*sizeof(char));              //Re-allocate space
					*capacity *= 2;
				}
				memcpy(*buffer + len, ngramVector->ngram[round], space*sizeof(char));
				len += space+1;
				(*buffer)[len-1] = ' ';
			}
			(*buffer)[len]='\0';

			ngram = safemalloc(len*sizeof(char));
			memcpy(ngram, *buffer, len);
			ngram[len-1] = '\0';

			if (*check > 0)
				printf("|");
			printf("%s", ngram);

			(*check)++;

			topK_Hashtable_insert(hashtable, ngram, len-1);
			topK_Hashtable_Check_LoadFactor(hashtable, len-1);
		}

#else			//WITH BLOOM FILTER
		else if (node->is_final) {

				for (; round <= i; round++) {
					if((space=(int)strlen(ngramVector->ngram[round])) >= *capacity - len-1) {
						*buffer = saferealloc(*buffer, 2 * (*capacity)*sizeof(char));              //Re-allocate space
						*capacity *= 2;
					}
					memcpy(*buffer + len, ngramVector->ngram[round], space*sizeof(char));
					len += space+1;
					(*buffer)[len-1] = ' ';
				}
				(*buffer)[len]='\0';

				ngram = safemalloc(len*sizeof(char));
				memcpy(ngram, *buffer, len);
				ngram[len-1] = '\0';

				if (!findInBloom(ngram)) {
					if (*check > 0)
						printf("|");
					printf("%s", ngram);

					(*check)++;

					topK_Hashtable_insert(hashtable, ngram, len-1);
					topK_Hashtable_Check_LoadFactor(hashtable, len-1);
				}
				else{
					free(ngram);
				}
			}
#endif

	}
}