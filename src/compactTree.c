#include "trieStructs.h"
#include "stack.h"
#include "errorMessages.h"
#include <string.h>
#include <stdlib.h>

void compactTree(){
	int i,j, mark = -1, top = 0, parentLength, childLength ;
	Stack stack;
	char *tempWord;
	TrieNode *temp, *hashtable = trieRoot->hashtable->Phashtable;
	
	initStack(&stack);
	for (i = 0; i < trieRoot -> hashtable->Buckets; i++){
		for (j = 0; j < hashtable[i].maxChildren - hashtable[i].emptySpace; j++){
			push(&stack, &(hashtable[i].children[j]));
		}
	}
	while(notEmpty(&stack)){
		if (stack.stack[stack.last]->emptySpace + 1 == stack.stack[stack.last]->maxChildren){
			
			/*Temp holds the children of the parent*/
			temp = &(stack.stack[stack.last]->children);
			childLength = strlen(temp[0]->word);

			/*tempWord holds the word of the parent node*/
			tempWord = stack.stack[stack.last]->word;
			parentLength = strlen(tempWord);

			/*Copy information of the child node to the parent node, do not copy information about array of offsets*/
			memcpy(stack.stack[stack.last], &temp[0], (sizeof(TrieNode)-2*sizeof(int)-sizeof(short *)));

			free(temp);

			/*Change size of the parent word*/
			tempWord = saferealloc(tempWord, strlen(stack.stack[stack.last]->word) + strlen(tempWord)+2);
			/*Copy to the parentWord the child word*/
			strncpy(tempWord + strlen(tempWord)+1, stack.stack[stack.last]->word, childLength+1);
			/*free current's node word (meaning child's word)*/
			free(stack.stack[stack.last]->word);
			/*Assing concatanated word to the current Node*/
			stack.stack[stack.last]->word = tempWord;


			/*Copy information of the child node to the parent node, do not copy information about array of offsets*/
			memcpy(stack.stack[stack.last], &temp[0], (sizeof(TrieNode)-2*sizeof(int)-sizeof(short *)));


			
			/*Fix array with lengths of each word*/
			if (stack.stack[stack.last]->offsets == NULL){
				stack.stack[stack.last]->offsets = safemalloc(sizeof(short)*stack.stack[stack.last]->maxOffsetSize);
			}
			else if (stack.stack[stack.last]->offsetsSize == stack.stack[stack.last]->maxOffsetSize){
				stack.stack[stack.last]->offsets = saferealloc(&(stack.stack[stack.last]->offsets), stack.stack[stack.last]->maxOffsetSize*2*sizeof(short));
				stack.stack[stack.last]->maxOffsetSize=stack.stack[stack.last]->maxOffsetSize*2;
			}
			if (stack.stack[stack.last]->is_final)
				stack.stack[stack.last]->offsets[stack.stack[stack.last]->offsetsSize-1] = -childLength;
			else
				stack.stack[stack.last]->offsets[stack.stack[stack.last]->offsetsSize-1] = childLength;
			stack.stack[stack.last]->offsetsSize++;
			/*Free child node, since it has merged with the parent*/
			//free(temp);
			
		}
		else if (stack.stack[stack.last]->emptySpace + 1 < stack.stack[stack.last]->maxChildren){
			temp = pop(&stack);
			for(i = 0; i <  temp->maxChildren-temp->emptySpace; i++){
				push(&stack, &(temp->children[i]));
			}			
		}
		else if (stack.stack[stack.last]->emptySpace == stack.stack[stack.last]->maxChildren){
			pop(&stack);
		}
	}
}
