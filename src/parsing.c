#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bursts.h"
#include "ngram.h"
#include "parsing.h"
#include "trieStructs.h"

void readInputFile(char *inputFile){
	int i;
	char buffer[1024], stop=0;
	NgramVector *ngram;
	FILE *fp;


	if ((fp = fopen(inputFile, "r")) == NULL){				// open graph file
		//perror(printError(2));
		exit(2);
	}

	ngram = initNgram();
	while (fgets(buffer, 1024, fp) != NULL){				// while there are still uninserted n-grams
		i = strlen(buffer);
		if (buffer[i-1] == '\n'){							// remove \n for easier handling
			buffer[i-1] = '\0';
		}															
		createNgram(ngram, buffer);
		trieInsertSort(ngram);					// or any other insertion function for the initial trie
	}
	deleteNgram(ngram);
	fclose(fp);
}



void readQueryFile(char *queryFile){
	int i;
	char buffer[1024], stop=0, command, burstFlag=1;
	NgramVector ngram;
	FILE *fp;


	if ((fp = fopen(queryFile, "r")) == NULL){				// open graph file
		//perror(printError(2));
		exit(2);
	}

	while (fgets(buffer, 1024, fp) != NULL){
		if (burstFlag){
			addBurst();
			burstFlag = 0;
		}

		if (buffer[0] == 'F'){
			burstFlag = 1;
		}
		else {
			i = strlen(buffer);
			if (buffer[i-1] == '\n'){							// remove \n for easier handling
				buffer[i-1] = '\0';
			}
			
			command = buffer[0];
			ngram = initNgram();
			createNgram(ngram, &buffer[2]);
			addCommand(command, ngram);
		}
	}
	fclose(fp);
}