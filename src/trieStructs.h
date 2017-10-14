#ifndef __TRIESTRUCTS_H__
#define __TRIESTRUCTS_H__

#include "ngram.h"

#define MINSIZE 100

typedef struct trieNode TrieNode;
typedef struct trieRoot TrieRoot;
typedef struct binaryResult BinaryResult;

struct trieNode{
    char* word;
    TrieNode *parentNode;
    TrieNode *children;
    int emptySpace;
    int maxChildren;
    int deletedChildren;
    char is_final;
};

struct trieRoot{
    TrieNode *root;
};

struct binaryResult{
    TrieNode * parent;
    char found;
};

TrieRoot *trieRoot;                                 //Global Variable

TrieRoot* trieRootInit();
TrieNode* trieNodeInit(char, TrieNode *, char *);
TrieNode* trieSearch(char**);
int trieInsertSort(char**);
void trieDeleteNgram(NgramVector*);
void trieFree();

#endif