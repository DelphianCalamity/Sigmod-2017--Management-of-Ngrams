#ifndef __TRIESTRUCTS_H__
#define __TRIESTRUCTS_H__

#include "ngram.h"

#define MINSIZE 500

typedef struct trieNode TrieNode;
typedef struct trieRoot TrieRoot;
typedef struct binaryResult BinaryResult;

struct trieNode{
    char* word;
    TrieNode *children;
    int emptySpace;
    int maxChildren;
    int deletedChildren;
    char is_final;
    char deleted;
    int visited;
};

struct trieRoot{
    TrieNode *root;
    int lastQuery;
};

struct binaryResult{
    int position;
    char found;
};

TrieRoot *trieRoot;                                 //Global Variable

void trieRootInit();
void trieNodeInit(char *, TrieNode *);
void trieBinarySearch(BinaryResult*, TrieNode *, char *);
void trieSearch(NgramVector *);
void trieSearch_Ngram(TrieNode*, int, int, NgramVector *, int *);
int trieInsertSort(NgramVector *);
void trieDeleteNgram(NgramVector*);
void trieRecursiveFree(TrieNode*);
void trieFree();

#endif
