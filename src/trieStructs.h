#ifndef __TRIESTRUCTS_H__
#define __TRIESTRUCTS_H__

#include "ngram.h"

#define MINSIZE 1
#define BUFFER_SIZE 10

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
    char deleted;
};

struct trieRoot{
    TrieNode *root;
};

struct binaryResult{
    int position;
    char found;
};

TrieRoot *trieRoot;                                 //Global Variable

void trieRootInit();
void trieNodeInit(char, TrieNode *, char *, TrieNode *);
void trieBinarySearch(BinaryResult*, TrieNode *, char *);
char* trieSearch(NgramVector *);
void trieSearch_Ngram(TrieNode*, int, int, NgramVector *, char*, int*);
int neededSpace(int, int, NgramVector*);
int trieInsertSort(NgramVector *);
void trieDeleteNgram(NgramVector*);
void trieFree();

#endif
