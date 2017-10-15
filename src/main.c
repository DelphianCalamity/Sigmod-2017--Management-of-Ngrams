#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "trieStructs.h"

int main(int argc, char **argv) {
    
    int i;
    char finit[10], fquery[10];
    
    //Read Main 's arguments
    if(argc < 5){
        printf("Correct syntax is: %s -i <init_file> -q <query_file>\n", argv[0]);
        return 1;
    }
    
    for(i=1; i<argc; i=i+2){
        
        if(strcmp(argv[i],"-i") == 0) {
            strcpy(finit, argv[i+1]);
        }
        else if(strcmp(argv[i],"-q") == 0){
            strcpy(fquery, argv[i+1]);
        }
        else {
            printf("Correct syntax is: %s -i <init_file> -q <query_file>\n", argv[0]);
            return 1;
        }
    }
 
    trieRoot = trieRootInit();                //Initializing Trie
    readInputFile(finit);                     //Input & Storing
    readQueryFile(fquery);
    
    return 0;
}
