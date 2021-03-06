#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bursts.h"
#include "parsing.h"
#include "errorMessages.h"
#include "CompactTrie/compactTree.h"


int main(int argc, char **argv) {

    int i;
    char *finit = NULL, *fquery = NULL;

    //Read Main 's arguments
    if(argc < 5){
        printf("Correct syntax is: %s -i <init_file> -q <query_file>\n", argv[0]);
        return 1;
    }

    for(i=1; i<argc; i=i+2){

        if(strcmp(argv[i],"-i") == 0)
            finit = argv[i+1];

        else if(strcmp(argv[i],"-q") == 0)
            fquery = argv[i+1];

        else {
            printf("Correct syntax is: %s -i <init_file> -q <query_file>\n", argv[0]);
            return 1;
        }
    }

    if (finit == NULL || fquery == NULL){
        getError(3);
        exit(2);
    }

	trieRootInit();                   		//Initializing Trie
	JobScheduler_Init();					//Initializing Job Scheduler

	readInputFile(finit);             		//Input & Storing

	if (TRIE_TYPE == STATIC) {           	//If trie is static then compress it
		trieCompactTree();
		commandsPtr = &executeStaticCommand;
		processBurstPtr = &processBurstStatic;
	}
	else {
		commandsPtr = &executeCommand;
		processBurstPtr = &processBurst;
	}

	readQueryFile(fquery);

//	Hashtable_print(trieRoot->hashtable);

	trieFree();
	JobScheduler_Destroy();

    return 0;
}

