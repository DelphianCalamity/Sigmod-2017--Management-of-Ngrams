#ifndef __COMPACT_H__
#define __COMPACT_H__

void trieCompactTree();
void trieCompactRoot(Stack*);
void trieSearch_Static(NgramVector* , int, int);
void trieSearch_Ngram_Static(TrieNode*, int, int, NgramVector*, char**, int*, int, char*, int);

#endif
