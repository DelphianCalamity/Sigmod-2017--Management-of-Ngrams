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
			
			/*Temp holds the only child of the current node*/
			temp = &(stack.stack[stack.last]->children[0]);
			childLength = strlen(temp->word);
			
			/*Free space for parent's children*/
			free(stack.stack[stack.last]->children);
			
			/*tempWord holds the word of the current node*/
			tempWord = stack.stack[stack.last]->word;
			parentLength = strlen(tempWord);
			
			/*Copy information of the child node to the parent node*/
			memcpy(stack.stack[stack.last], temp, (sizeof(TrieNode)-2*sizeof(int)-sizeof(short *)));
			
			/*Change size of the parent word*/
			tempWord = saferealloc(tempWord, strlen(stack.stack[stack.last]->word) + strlen(tempWord)+2);
			/*Copy to the parentWord the child word*/
			strncpy(tempWord + strlen(tempWord)+1, stack.stack[stack.last]->word, childLength+1);
			/*free current's node word*/
			free(stack.stack[stack.last]->word);
			/*Assing concatanated word to the current Node*/
			stack.stack[stack.last]->word = tempWord;			
			
			/*Fix array with lengths of each word*/
			if (stack.stack[stack.last]->offsets == NULL){
				stack.stack[stack.last]->offsets = safemalloc(sizeof(short)*10);
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
			free(temp);
			
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
