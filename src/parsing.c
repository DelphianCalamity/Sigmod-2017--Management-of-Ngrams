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
	if ((len=getline(&buffer, &size, fp)) != -1){				//Identify Trie's type (Static or Dynamic)
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
	int i=0;
	size_t size=0;
	char *buffer=NULL;
	char command;
	NgramVector *ngram;

	if ((fp = fopen(queryFile, "r")) == NULL) {                  // open query file
		getError(3);
		exit(2);
	}

	j=-1;
	trie_buffer_Init();
	burst_init();

    ssize_t len;
//	clock_t start, end;
//	double cpu_time_used;

//	start = clock();
	while ((len=getline(&buffer, &size, fp)) != -1) {

		if (buffer[0] == 'F') {

			if (len > 2) {
				buffer[len] = '\0';
				burst[i].k = atoi(buffer+2);
			}
			else burst[i].k = 0;

			pthread_join(burst_processor, NULL);

			if (pthread_create(&burst_processor, NULL, processBurst, &i)){
				perror("pthread_create");
				exit(1);
			}
			/*** ***** ***/
			i = (i+1)%2;

//			end = clock();
//			cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
//			printf("Parsing_Time = %f\n", cpu_time_used);

//			start = clock();
		}

		else {
            command = buffer[0];
			ngram = initNgram();
			createNgram(ngram, &buffer[2], len-2);
			addCommand(command, ngram, i);
		}

//		burst[i].numOfJobs = 0;

	}

	if (buffer != NULL){
		free(buffer);
	}
	fclose(fp);

	pthread_join(burst_processor, NULL);

	trie_buffer_Destroy();

	free(burst[0].jobs);
	free(burst[1].jobs);
}


