#include "trieStructs.h"
#include "stack.h"
#include "errorMessages.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void compactTree(){
	int i,j, parentLength, childLength ;
	Stack stack;
	char *tempWord;
	TrieNode *hashtable = trieRoot->hashtable->Phashtable, *parent, *child, *tempchildren;

	initStack(&stack);
	for (i = 0; i < trieRoot -> hashtable->Buckets; i++){
		for (j = 0; j < hashtable[i].maxChildren - hashtable[i].emptySpace; j++){
			push(&stack, &(hashtable[i].children[j]));
			printf("kati valame %s\n\n\n", hashtable[i].children[j].word);
		}
	}
	for (i=0; i < stack.last; i++){
		printf("words in stack are |%s|\n", stack.stack[stack.last]->word);
	}
	while(notEmpty(&stack)){
		if (stack.stack[stack.last]->emptySpace + 1 == stack.stack[stack.last]->maxChildren){


			parent = stack.stack[stack.last];
			child = &parent->children[0];

			parentLength = strlen(parent -> word);
			childLength = strlen(child -> word);


			printf("\n\n\n word is %s\n", parent->word);
			printf("word in node is %s\n\n", child->word);
			printf("parent is %d:", stack.stack[stack.last]->offsetsSize);

			getchar();
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

			getchar();


			/*Fix array with lengths of each word*/
			if (stack.stack[stack.last]->offsets == NULL){
				stack.stack[stack.last]->offsets = safemalloc(sizeof(short)*stack.stack[stack.last]->maxOffsetSize);
			}
			else if (stack.stack[stack.last]->offsetsSize == stack.stack[stack.last]->maxOffsetSize){
				stack.stack[stack.last]->offsets = saferealloc(stack.stack[stack.last]->offsets, stack.stack[stack.last]->maxOffsetSize*2*sizeof(short));
				stack.stack[stack.last]->maxOffsetSize=stack.stack[stack.last]->maxOffsetSize*2;
			}
			if (stack.stack[stack.last]->is_final == 1)
				stack.stack[stack.last]->offsets[stack.stack[stack.last]->offsetsSize-1] = -childLength;
			else
				stack.stack[stack.last]->offsets[stack.stack[stack.last]->offsetsSize-1] = childLength;
			stack.stack[stack.last]->offsetsSize++;
			/*Free child node, since it has merged with the parent*/
			//free(temp);
			// printf("parent is %d:", stack.stack[stack.last]->offsetsSize);
			// for (i = 0; i < stack.stack[stack.last]->offsetsSize; i++){
			// 	for (j=0; j<stack.stack[stack.last]->offsets[i]; j++)
			// 		putchar(stack.stack[stack.last]->word[i]);
			// }

		}
		else if (stack.stack[stack.last]->emptySpace + 1 < stack.stack[stack.last]->maxChildren){
			printf("mpike \n" );
			parent = pop(&stack);
			for(i = 0; i <  parent->maxChildren-parent->emptySpace; i++){
				push(&stack, &(parent->children[i]));
			}
		}
		else if (stack.stack[stack.last]->emptySpace == stack.stack[stack.last]->maxChildren){
			printf("mpike se final %d\n", stack.stack[stack.last]->is_final);
			pop(&stack);
		}
	}
}
