

typedef struct trieNode{
    char* word;
    trieNode *parentNode;
    trieNode **children;
    char is_final;
}TrieNode;


typedef struct trieRoot{
    TrieNode *root;
}TrieRoot;