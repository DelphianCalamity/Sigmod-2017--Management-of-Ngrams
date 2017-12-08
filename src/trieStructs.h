#ifndef __TRIESTRUCTS_H__
#define __TRIESTRUCTS_H__

#include "ngram.h"
#include "stack.h"
#include "Hashtable/Hashtable.h"

//#define BLOOM

#define MINSIZE 10
#define FACTOR 0.3
#define BUFFER_SIZE 100

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

	#ifndef BLOOM
    	int visited;
	#endif

    short *offsets;
    int offsetsSize;
};

struct trieRoot{
	Hashtable_Info_ptr hashtable;

	#ifndef BLOOM
		int lastQuery;
	#endif
};

struct binaryResult{
    int position;
    char found;
};

char TRIE_TYPE;
TrieRoot *trieRoot;                                 	//Global Variable
void (*SearchPtr)(NgramVector*);

void trieRootInit();
void trieNodeInit(char *, TrieNode *);
void trieBinarySearch(BinaryResult*, TrieNode *, char *);
void trieSearch(NgramVector *);
void trieSearch_Ngram(TrieNode*, int, int, NgramVector *, char**, int*, int *);
void trieCompactSpace(TrieNode*);
int trieInsertSort(NgramVector *);
void trieMakeSpace(BinaryResult*, TrieNode*, char*);
void trieDeleteNgram(NgramVector*);
void trieRecursiveFree(TrieNode*);
void trieFree();

#endif
