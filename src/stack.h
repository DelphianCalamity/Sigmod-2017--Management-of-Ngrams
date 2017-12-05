#ifndef __STACK_H__
#define  __STACK_H__

#include "trieStructs.h"
#include <stdbool.h>

typedef struct trieNode TrieNode;
typedef struct stack Stack;

struct stack{
	TrieNode **stack;
	int size;
	int last;
};

void initStack(Stack *);
void push(Stack *, TrieNode *);
bool notEmpty(Stack *);
TrieNode * pop(Stack *);
void deleteStack(Stack *);
void printStack(Stack *);

#endif
