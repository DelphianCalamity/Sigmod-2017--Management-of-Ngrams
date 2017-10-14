#ifndef NGRAM_H
#define	NGRAM_H

#define WORD_SIZE 50

typedef struct word Word;
typedef Word* Wordptr;

typedef struct ngram Ngram;
typedef Ngram* Ngramptr;

struct ngram {                          //A list of words
    Ngramptr start;
    Ngramptr end;
    int size;
};

struct word {                           //node of list - a word
    char word[WORD_SIZE];
    Wordptr next;
    Wordptr prev;
};

Ngramptr Ngram_create(void);
Ngramptr Ngram_Insert_End(Ngramptr, char*);
void Ngram_print(Ngramptr);
void Ngram_destroy(Ngramptr);

#endif
