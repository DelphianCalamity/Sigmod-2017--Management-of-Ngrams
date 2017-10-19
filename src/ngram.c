#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ngram.h"
#include "errorMessages.h"

NgramVector * initNgram(void){										// creates and initializes the ngram struct
	NgramVector *ngramVector;

	if ((ngramVector = malloc(sizeof(NgramVector))) == NULL){
		getError(1);
		exit(1);
	}

	if ((ngramVector->ngram = malloc(INIT_SIZE*sizeof(char *))) == NULL){
		getError(1);
		exit(1);
	}
	ngramVector->capacity = INIT_SIZE;
	ngramVector->words = 0;

	return ngramVector;
}


void createNgram(NgramVector *ngramVector, char *buffer){			// creates an ngram from the buffer
	char *token;
	int i=0, s, e;

	s = 0;
	e = 0;
	ngramVector->words = 0;
	for (i=0; i<(strlen(buffer)); i++){
		if (buffer[i] != ' ' && buffer[i] != '\n')
			e++;
		else{
			ngramVector->ngram[ngramVector->words] = malloc((e-s+1)*sizeof(char));
			memcpy(ngramVector->ngram[ngramVector->words], &buffer[s], e-s);
			ngramVector->ngram[ngramVector->words][e-s]='\0';
			ngramVector->words++;
			s = e+1;
		}
	}
	// ngramVector->ngram[ngramVector->words] = malloc(e-s+1);
	// memcpy(ngramVector->ngram[ngramVector->words], &buffer[s], e-s);
	// ngramVector->ngram[ngramVector->words][e-s]='\0';
	// ngramVector->words++;
	// ngramVector->ngram[0] = strtok(buffer, " ");					// first word
	// ngramVector->words = 1;											// current number of words in the vector
	// while ((token = strtok(NULL, " ")) != NULL){					// while there are still some words
	// 	if (ngramVector->words == ngramVector->capacity){
	// 		if ((ngramVector->ngram = realloc(ngramVector->ngram, 2*ngramVector->capacity)) == NULL){			// not enough space in vector, double it
	// 			getError(2);
	// 			exit(1);
	// 		}
	// 		ngramVector->capacity *= 2;
	// 	}
	// 	ngramVector->ngram[ngramVector->words] = token;
	// 	(ngramVector->words)++;
	// }
}


void deleteNgram(NgramVector *ngramVector){							// deletes the ngram struct, does not touch the words of the ngram
	free(ngramVector->ngram);
	free(ngramVector);
}


void deleteWords(NgramVector *ngramVector){
	int i;

	for (i=0; i<ngramVector->words; i++)
		free(ngramVector->ngram[i]);
}
