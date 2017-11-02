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

void createNgram(NgramVector *ngramVector, char *buffer, ssize_t size){			// creates an ngram from the buffer
	
    int s=0, e;

	ngramVector->words = 0;

    for (e=0; e<size; e++) {
		if (!(buffer[e] != ' ' && buffer[e] != '\n')) {

			if (s != e){
                if (ngramVector->words == ngramVector->capacity){
					ngramVector->capacity *= 2;
					if ((ngramVector->ngram = realloc(ngramVector->ngram, ngramVector->capacity*sizeof(char*))) == NULL) {
                        getError(2);
                        exit(2);
                    }
                }

				ngramVector->ngram[ngramVector->words] = malloc((e-s+1)*sizeof(char));
				memcpy(ngramVector->ngram[ngramVector->words], &buffer[s], e-s);
				ngramVector->ngram[ngramVector->words][e-s]='\0';
				ngramVector->words++;
			}
			s = e+1;
		}
	}
}


void deleteNgram(NgramVector *ngramVector){							        // deletes the ngram struct, does not touch the words of the ngram
	//deleteWords(ngramVector);
	free(ngramVector->ngram);
	free(ngramVector);
}


void deleteWords(NgramVector *ngramVector){
	int i;

	for (i=0; i<ngramVector->words; i++)
		if (ngramVector->ngram[i] != NULL)
			free(ngramVector->ngram[i]);
}
