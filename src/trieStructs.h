#ifndef __TRIESTRUCTS_H__
#define __TRIESTRUCTS_H__

typedef struct trieNode TrieNode;
typedef struct trieRoot TrieRoot;

struct trieNode{
	char* word;
	TrieNode *parentNode;
	TrieNode **children;
    int currentChildren;
    int maxChildren;
	char is_final;
}e;


struct trieRoot{
	TrieNode *root;
};

TrieRoot * rootInit();
TrieNode * trieNodeInit(char, TrieNode *, char *);

#endif