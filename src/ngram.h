#ifndef __NGRAM_H__
#define __NGRAM_H__

#define INIT_SIZE 10

typedef struct ngramVector{
	char **ngram;
	int capacity;
	int words;
}NgramVector;

NgramVector * initNgram(void);
void createNgram(NgramVector*, char*);
void deleteNgram(NgramVector*);
void deleteWords(NgramVector*);

#endif