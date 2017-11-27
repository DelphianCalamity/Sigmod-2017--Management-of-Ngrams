#include "trieStructs.h"
#include "stack.h"
#include "errorMessages.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void compactTree(){
	int i,j, parentLength, childLength ;
	Stack stack;
	char *tempWord, *tempWord2;
	TrieNode *hashtable = trieRoot->hashtable->Phashtable, *parent, *child, *tempchildren;

	initStack(&stack);
	for (i = 0; i < trieRoot -> hashtable->Buckets; i++){
		for (j = 0; j < hashtable[i].maxChildren - hashtable[i].emptySpace; j++){
			push(&stack, &(hashtable[i].children[j]));
		}
	}

	while(notEmpty(&stack)){
		if (stack.stack[stack.last]->emptySpace + 1 == stack.stack[stack.last]->maxChildren){


			parent = stack.stack[stack.last];
			child = &parent->children[0];

			parentLength = strlen(parent -> word);
			childLength = strlen(child -> word);


			/*Change size of the parent word*/
			parent->word = saferealloc(parent->word, parentLength + childLength + 1);
			/*Copy to the parentWord the child word*/
			strncpy(parent->word + (parentLength)*sizeof(char), child->word, childLength);
			parent->word[parentLength+childLength] ='\0';
			/*free current's node word (meaning child's word)*/
			free(child->word);

			tempWord =  parent->word;
			tempchildren = parent->children;

			memcpy(parent, child, sizeof(TrieNode)-2*sizeof(int)-sizeof(short *));

			free(tempchildren);
			/*Assigning concatanated word to the current Node*/
			parent->word = tempWord;

			/*Fix array with lengths of each word*/
			if (parent->offsets == NULL){
				parent->offsets = safemalloc(sizeof(short)*parent->maxOffsetSize);
			}
			else if (parent->offsetsSize == parent->maxOffsetSize){

				parent->offsets = saferealloc(parent->offsets, parent->maxOffsetSize*2*sizeof(short));
				parent->maxOffsetSize=parent->maxOffsetSize*2;
			}
			stack.stack[stack.last]->offsetsSize++;
			if (stack.stack[stack.last]->is_final == 1)
				parent->offsets[parent->offsetsSize-1] = -childLength;
			else
				stack.stack[stack.last]->offsets[parent->offsetsSize-1] = childLength;

		}
		else if (stack.stack[stack.last]->emptySpace + 1 < stack.stack[stack.last]->maxChildren){
			parent = pop(&stack);
			for(i = 0; i <  parent->maxChildren-parent->emptySpace; i++){
				push(&stack, &(parent->children[i]));
			}
		}
		else if (stack.stack[stack.last]->emptySpace == stack.stack[stack.last]->maxChildren){
			pop(&stack);
		}

	}
	deleteStack(&stack);
}
