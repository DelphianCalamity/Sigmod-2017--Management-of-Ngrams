#ifndef __TRIESTRUCTS_H__
#define __TRIESTRUCTS_H__

#include "ngram.h"
#include "stack.h"
#include "Hashtable/Hashtable.h"

#define MINSIZE 10
#define FACTOR 0.3

#define DYNAMIC 1
#define STATIC 0

typedef struct hashtable_info* Hashtable_Info_ptr;		//Forward Declaration

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
	Hashtable_Info_ptr hashtable;
    int lastQuery;
	int lastBurst;
};

struct binaryResult{
    int position;
    char found;
};

char TRIE_TYPE;
TrieRoot *trieRoot;                                 	//Global Variable

void trieRootInit();
void trieNodeInit(char *, TrieNode *, TrieNode *);
void trieBinarySearch(BinaryResult*, TrieNode *, char *);
void trieSearch(NgramVector *);
void trieSearch_Ngram(TrieNode*, int, int, NgramVector *, int *);
void trieCompactSpace(TrieNode*);
int trieInsertSort(NgramVector *);
void trieMakeSpace(BinaryResult*, TrieNode*, char*);
void trieDeleteNgram(NgramVector*);
void trieRecursiveFree(TrieNode*);
void trieFree();

#endif
