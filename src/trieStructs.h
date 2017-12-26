#ifndef __TRIESTRUCTS_H__
#define __TRIESTRUCTS_H__

#include "ngram.h"
#include "stack.h"
#include "Hashtable/Hashtable.h"
#include "Threads/JobScheduler.h"

#define MINSIZE 10
#define FACTOR 0.3
#define BUFFER_SIZE 100

#define DYNAMIC 1
#define STATIC 0

typedef struct hashtable_info* Hashtable_Info_ptr;		//Forward Declaration
typedef struct jobScheduler JobScheduler;

typedef struct trieNode TrieNode;
typedef struct trieRoot TrieRoot;
typedef struct binaryResult BinaryResult;
typedef struct queryBuffer QueryBuffer;

struct trieNode{
    char* word;
    TrieNode *children;
    int emptySpace;
    int maxChildren;
    int deletedChildren;
    char is_final;
	char deleted;

    short *offsets;                                     // For Static Tries
    int offsetsSize;

	int A_version;                                      // For threads
	int D_version;
};

struct trieRoot{
	Hashtable_Info_ptr hashtable;
	int current_version;
};

struct binaryResult{
    int position;
    char found;
};

struct queryBuffer {
	int capacity;
	char** buffer;
	int* capacities;
	int *sizes;
};

char TRIE_TYPE;
TrieRoot *trieRoot;                                 	//Global Variable
JobScheduler jobScheduler;
void (*SearchPtr)(NgramVector*, int, int);
QueryBuffer queryBuffer;

void trieRootInit();
void trieNodeInit(char *, TrieNode *);
void trieBinarySearch(BinaryResult*, TrieNode *, char *);
void trieSearch(NgramVector *, int, int);
void trieSearch_Ngram(TrieNode*, int, int, NgramVector *, char**, int*, int, int, char*);
void trieCompactSpace(TrieNode*);
int trieInsertSort(NgramVector *);
void trieMakeSpace(BinaryResult*, TrieNode*, char*);
void trieDeleteNgram(NgramVector*);
void trieFakeDeleteNgram(NgramVector*);
void trieRecursiveFree(TrieNode*);
void trie_buffer_Init();
void trie_buffer_Destroy();
void trieFree();

#endif
