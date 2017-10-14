#ifndef __TRIESTRUCTS_H__
#define __TRIESTRUCTS_H__

#include "ngram.h"

#define MINSIZE 100

typedef struct trieNode TrieNode;
typedef struct trieRoot TrieRoot;

struct trieNode{
    char* word;
    TrieNode *parentNode;
    TrieNode **children;
    int emptySpace;
    int maxChildren;
    int deletedChildren;
    char is_final;
};

struct trieRoot{
    TrieNode *root;
};

TrieRoot* trieRootInit();
TrieNode* trieNodeInit(char, TrieNode *, char *);
TrieNode* trieSearch(TrieRoot* trieRoot, Ngramptr ngram);
int trieInsertSort(TrieRoot* trieRoot, Ngramptr ngram);


#endif