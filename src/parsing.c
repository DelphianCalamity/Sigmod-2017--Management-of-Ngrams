#include <stdio.h>
#include <stdlib.h>
#include "bursts.h"
#include "errorMessages.h"
#include "parsing.h"

void readInputFile(char *inputFile) {

	FILE *fp;
	size_t size=0;
	char *buffer=NULL;
	NgramVector *ngram;

	if ((fp = fopen(inputFile, "r")) == NULL){                  // open init file
		getError(3);
		exit(3);
	}

	ngram = initNgram();

    ssize_t len;
	if((getline(&buffer, &size, fp)) != -1){				//Identify Trie's type (Static or Dynamic)
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

    if (buffer != NULL){
        free(buffer);
	}
	fclose(fp);
}


void readQueryFile(char *queryFile){

	FILE *fp;
	size_t size=0;
	char *buffer=NULL;
	char command;
	NgramVector *ngram;

	if ((fp = fopen(queryFile, "r")) == NULL) {                  // open query file
		getError(3);
		exit(2);
	}

	burst_init();
	trie_buffer_Init();

    ssize_t len;
	while ((len=getline(&buffer, &size, fp)) != -1) {

		if (buffer[0] == 'F') {

			if (len > 2) {
				buffer[len] = '\0';
				burst.k = atoi(buffer+2);
			}
			else burst.k = 0;

			(*processBurstPtr)();

			burst.numOfJobs = 0;
		}

		else {
            command = buffer[0];
			ngram = initNgram();
			createNgram(ngram, &buffer[2], len-2);
			addCommand(command, ngram);
		}
	}

	trie_buffer_Destroy();

    if(buffer != NULL){
        free(buffer);
	}
	fclose(fp);

	// wake all workers to be destroyed

	pthread_mutex_lock(&jobScheduler.queue_mutex);
	jobScheduler.kill = 1;
	pthread_mutex_unlock(&jobScheduler.queue_mutex);
	pthread_cond_broadcast(&jobScheduler.queue_empty);

	int i;
	for (i=0; i < jobScheduler.thread_pool_size; i++)
		pthread_join(jobScheduler.workers[i], NULL);
}


