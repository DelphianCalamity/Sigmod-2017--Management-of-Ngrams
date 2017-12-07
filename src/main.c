#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "bursts.h"
#include "parsing.h"
#include "errorMessages.h"
#include "trieStructs.h"
#include "BloomFilter/bloomFilter.h"
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
	readInputFile(finit);             		//Input & Storing

	if (TRIE_TYPE == STATIC) {           	//If trie is static then compress it
		trieCompactTree();
		SearchPtr = &trieSearch_Static;		//Pointer to function
	}
	else SearchPtr = &trieSearch;

    readQueryFile(fquery);

#ifdef BLOOM
	bloomfilter = safemalloc(BLOOMSIZE);
#endif

	processBursts();

#ifdef BLOOM
	free(bloomfilter);
#endif
    
//	Hashtable_print(trieRoot->hashtable);

	trieFree();

    return 0;
}

