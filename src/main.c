#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "trieStructs.h"
#include "parsing.h"
#include "errorMessages.h"

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
            if ((finit=malloc(strlen(argv[i+1])*sizeof(char)))==NULL){
                getError(1);
                exit(1);
            }
            strcpy(finit, argv[i+1]);
        }
        else if(strcmp(argv[i],"-q") == 0){
            if ((fquery=malloc(strlen(argv[i+1])*sizeof(char)))==NULL){
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


    /*
    //some testing of binary search
    BinaryResult br;
    TrieNode node;
    trieNodeInit(0, NULL, "boo", &node);

    trieNodeInit(0, NULL, "aa", &node.children[0]);
    node.emptySpace--;


    trieNodeInit(0, NULL, "ab", &node.children[1]);
    node.emptySpace--;

    trieBinarySearch(&br, &node, "b");

    printf("found=%d\nposition=%d\n", br.found, br.position);
    */

    trieRootInit();                 //Initializing Trie
    readInputFile(finit);           //Input & Storing
    //readQueryFile(fquery);
    return 0;
}
