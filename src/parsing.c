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
    while ((len=getline(&buffer, &size, fp)) != -1){
		createNgram(ngram, buffer, len);
		trieInsertSort(ngram);                                  // or any other insertion function for the initial trie
		deleteWords(ngram);
	}
	deleteNgram(ngram);
    
    if(buffer != NULL)
        free(buffer);
	fclose(fp);
}


void readQueryFile(char *queryFile){

	size_t size=0;
	char *buffer=NULL;
	char command; //, burstFlag=1;
	NgramVector *ngram;
	FILE *fp;

	if ((fp = fopen(queryFile, "r")) == NULL){                  // open query file
		getError(3);
		exit(2);
	}

    ssize_t len;
	while ((len=getline(&buffer, &size, fp)) != -1){
		
        //if (burstFlag){
		//	addBurst();
		//	burstFlag = 0;
		//}

		if (buffer[0] == 'F'){
            //processBursts();
            //burstFlag = 1;
        }
		else {                                                  //Ignoring burst list for the first part

            command = buffer[0];
			ngram = initNgram();
			createNgram(ngram, &buffer[2], len-2);

            if (command == 'A'){
                trieInsertSort(ngram);
            }
            else if (command == 'D')
                trieDeleteNgram(ngram);

            else if (command == 'Q')
                trieSearch(ngram);

            else {
                printf("Wrong query\n");
                exit(1);
            }
            deleteWords(ngram);
            deleteNgram(ngram);

			//addCommand(command, ngram);//
		}
	}

    if(buffer != NULL)
        free(buffer);
	fclose(fp);
}
