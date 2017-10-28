#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "errorMessages.h"
#include "stack.h"
#include "trieStructs.h"

void initStack(Stack *s){
	if ((s->stack=malloc(10*sizeof(TrieNode *)))==NULL){
		getError(1);
		exit(1);
	}
	s->size=10;
	s->last=-1;
}


void push(Stack *s, TrieNode *node){
	TrieNode **temp;

	//printf("Gonna push, size is:%d and last is:%d\n", s->size, s->last);
	if (s->last == (s->size-1)){
		//printf("Realloc with new size:%d\n", s->size + (s->size)/10 + 1);
		s->size = s->size + (s->size)/10 + 1;
		if ((temp=realloc(s->stack, (s->size)*sizeof(TrieNode *)))==NULL){
			getError(2);
			exit(2);
		}
		s->stack=temp;
	}
	s->last++;
	s->stack[s->last]=node;
	//printf("After push, size is:%d and last is:%d\n", s->size, s->last);
}

bool notEmpty(Stack *s){
	if (s->last == -1)
		return 0;
	return 1;
}


TrieNode * pop(Stack *s){
	//printf("In start of pop size is:%d and last is:%d\n", s->size, s->last);
	if (!notEmpty(s)){
		getError(4);
		exit(4);
	}
	s->last--;
	//printf("In end of pop size is:%d and last is:%d\n", s->size, s->last);
	return (s->stack[s->last+1]);
}

void deleteStack(Stack *s){
	free(s->stack);
}


 void printStack(Stack *S){
	int i;

	printf("S has :");
	if (S->last==-1){
		printf("\n");
		return;
	}
	for (i=0; i<=S->last; i++){
		printf("%s-->", S->stack[i]->word);
	}
	printf("\n");
 }
