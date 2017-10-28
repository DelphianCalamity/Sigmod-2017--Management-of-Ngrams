#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "trieStructs.h"
#include "parsing.h"
#include "errorMessages.h"
#include "bursts.h"
#include <unistd.h>

int main(int argc, char **argv) {

    int i;
    char *finit, *fquery;

    //Read Main 's arguments
    if(argc < 5){
        printf("Correct syntax is: %s -i <init_file> -q <query_file>\n", argv[0]);
        return 1;
    }

    for(i=1; i<argc; i=i+2){

        if(strcmp(argv[i],"-i") == 0) {
            if ((finit=malloc((strlen(argv[i+1])+1)*sizeof(char)))==NULL){
                getError(1);
                exit(1);
            }
            strcpy(finit, argv[i+1]);
        }
        else if(strcmp(argv[i],"-q") == 0){
            if ((fquery=malloc((strlen(argv[i+1])+1)*sizeof(char)))==NULL){
                getError(1);
                exit(1);
            }
            strcpy(fquery, argv[i+1]);
        }
        else {
            printf("Correct syntax is: %s -i <init_file> -q <query_file>\n", argv[0]);
            return 1;
        }
    }

    trieRootInit();                   //Initializing Trie
    readInputFile(finit);             //Input & Storing
    readQueryFile(fquery);

    //executeBurstCommands(burstListStart);

    return 0;
}

