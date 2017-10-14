#ifndef __TRIESTRUCTS_H__
#define __TRIESTRUCTS_H__

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

TrieRoot *trieRoot;                                 //Global Variable

TrieRoot* trieRootInit();
TrieNode* trieNodeInit(char, TrieNode *, char *);
TrieNode* trieSearch(char**);
int trieInsertSort(char**);
void trieFree();

#endif