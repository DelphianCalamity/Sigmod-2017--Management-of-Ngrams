#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"
#include "errorMessages.h"
#include "trieStructs.h"
#include "compactTree.h"

//void trieCompactTree(){
//	int i,j, parentLength, childLength ;
//	Stack stack;
//	char *tempWord, *tempWord2;
//	TrieNode *hashtable = trieRoot->hashtable->Phashtable, *parent, *child, *tempchildren;
//
//	initStack(&stack);
//	for (i = 0; i < trieRoot -> hashtable->Buckets; i++){
//		for (j = 0; j < hashtable[i].maxChildren - hashtable[i].emptySpace; j++){
//			push(&stack, &(hashtable[i].children[j]));
//		}
//	}
//
//	while(notEmpty(&stack)){
//		if (stack.stack[stack.last]->emptySpace + 1 == stack.stack[stack.last]->maxChildren){
//
//
//			parent = stack.stack[stack.last];
//			child = &parent->children[0];
//
//			parentLength = strlen(parent -> word);
//			childLength = strlen(child -> word);
//
//
//			/*Change size of the parent word*/
//			parent->word = saferealloc(parent->word, parentLength + childLength + 1);
//			/*Copy to the parentWord the child word*/
//			strncpy(parent->word + (parentLength)*sizeof(char), child->word, childLength);
//			parent->word[parentLength+childLength] ='\0';
//			/*free current's node word (meaning child's word)*/
//			free(child->word);
//
//			tempWord =  parent->word;
//			tempchildren = parent->children;
//
//			memcpy(parent, child, sizeof(TrieNode)-2*sizeof(int)-sizeof(short *));
//
//			free(tempchildren);
//			/*Assigning concatanated word to the current Node*/
//			parent->word = tempWord;
//
//			/*Fix array with lengths of each word*/
//			if (parent->offsets == NULL){
//				parent->offsets = safemalloc(sizeof(short)*parent->maxOffsetSize);
//			}
//			else if (parent->offsetsSize == parent->maxOffsetSize){
//
//				parent->offsets = saferealloc(parent->offsets, parent->maxOffsetSize*2*sizeof(short));
//				parent->maxOffsetSize=parent->maxOffsetSize*2;
//			}
//			stack.stack[stack.last]->offsetsSize++;
//			if (stack.stack[stack.last]->is_final == 1)
//				parent->offsets[parent->offsetsSize-1] = -childLength;
//			else
//				stack.stack[stack.last]->offsets[parent->offsetsSize-1] = childLength;
//
////			printf("%s --- ",stack.stack[stack.last]->word);
////			for(i=0; i<stack.stack[stack.last]->offsetsSize; i++)
////				printf("%d ",stack.stack[stack.last]->offsets[i]);
////			printf("\n");
//		}
//		else if (stack.stack[stack.last]->emptySpace + 1 < stack.stack[stack.last]->maxChildren){
//			parent = pop(&stack);
//			for(i = 0; i <  parent->maxChildren-parent->emptySpace; i++){
//				push(&stack, &(parent->children[i]));
//			}
//		}
//		else if (stack.stack[stack.last]->emptySpace == stack.stack[stack.last]->maxChildren){
//			pop(&stack);
//		}
//
//	}
//	deleteStack(&stack);
//}


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

void trieCompactRoot(Stack *stack) {														//Splitting compact into 2 functions is not needed after all

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
			wLen = strlen(parent->word);
			wCapacity = wLen + 1;
			oCapacity = 5;
			oBuffer = safemalloc(oCapacity * sizeof(short));
			oBuffer[0] = wLen;
			oLen = 1;
		}
		while (tempSize == 1) {																//While parents have only one child

			prePrevious = previous;
			previous = parent->children;
			parent = &parent->children[0];

			wSpace = strlen(parent->word);
			while (wSpace >= wCapacity-wLen-1) {
				wCapacity *= 2;
				wBuffer = saferealloc(wBuffer, wCapacity * sizeof(char));           		//Re-allocate space for word-Buffer
			}
			if (oCapacity == oLen) {
				oCapacity *= 2;
				oBuffer = saferealloc(oBuffer, oCapacity * sizeof(short));          		//Re-allocate space for offset-Buffer
			}

			memcpy(wBuffer + wLen, parent->word, wSpace*sizeof(char));  wLen += wSpace;		//Copy current node's word at wBuffer
			wBuffer[wLen]='\0';

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
//			printf("%s --- ",wBuffer);
//			for(i=0; i<oLen; i++)
//				printf("%d ",oBuffer[i]);
//			printf("\n");
		}

		if (node->emptySpace != node->maxChildren) {							//If node is not a leaf - has more than one children - push them in stack
			for(i = 0; i <  node->maxChildren-node->emptySpace; i++)
				push(stack, &(node->children[i]));
		}
	}
}