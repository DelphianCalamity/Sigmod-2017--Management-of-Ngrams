#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bursts.h"
#include "errorMessages.h"
#include "ngram.h"
#include "parsing.h"
#include "trieStructs.h"

void readInputFile(char *inputFile) {

	size_t size=0;
	char *buffer=NULL;
	NgramVector *ngram;
	FILE *fp;

	if ((fp = fopen(inputFile, "r")) == NULL){                  // open init file
		getError(3);
		exit(3);
	}

	ngram = initNgram();

    ssize_t len;
	if((len=getline(&buffer, &size, fp)) != -1){				//Identify Trie's type (Static or Dynamic)
		if(buffer[0] == 'S')
			TRIE_TYPE = STATIC;
		else TRIE_TYPE = DYNAMIC;
	}

	while ((len=getline(&buffer, &size, fp)) != -1){
		createNgram(ngram, buffer, len);
		trieInsertSort(ngram);                                  // or any other insertion function for the initial trie
		deleteWords(ngram);
	}
	deleteNgram(ngram);

    if(buffer != NULL){
        free(buffer);
	}
	fclose(fp);
}


void readQueryFile(char *queryFile){

	FILE *fp;
	size_t size=0;
	char *buffer=NULL;
	char command, burstFlag=1;
	NgramVector *ngram;

	if ((fp = fopen(queryFile, "r")) == NULL) {                  // open query file
		getError(3);
		exit(2);
	}

	burst_init();

    ssize_t len;
	while ((len=getline(&buffer, &size, fp)) != -1) {

        if (burstFlag)
			burstFlag = 0;

		if (buffer[0] == 'F') {

			if (len > 2) {
				buffer[len] = '\0';
				burst.k = atoi(buffer+2);
			}
			else burst.k = 0;

            burstFlag = 1;
			executeBurstCommands();								//frees space too

			burst.numOfJobs = 0;
		}

		else {
            command = buffer[0];
			ngram = initNgram();
			createNgram(ngram, &buffer[2], len-2);

			addCommand(command, ngram);
			//JobScheduler_SubmitJob(&burst.jobs[i]);			//Replaces the command on top
		}
	}

	free(burst.jobs);

    if(buffer != NULL){
        free(buffer);
	}
	fclose(fp);
}
