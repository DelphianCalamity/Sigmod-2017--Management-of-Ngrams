#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "trieStructs.h"
#include "ngram.h"


int fpeek(FILE *fp);
void insert_Ngrams_In_Trie(FILE* fp, TrieRoot* trieRoot);

int main(int argc, char **argv) {
    
    int i;
    FILE *finit, *fquery;
    TrieRoot *trieRoot;

    
    //Read Main 's arguments
    if(argc < 5){
        printf("Correct syntax is: %s -i <init_file> -q <query_file>\n", argv[0]);
        return 1;
    }
    
    for(i=1; i<argc; i=i+2){
        
        if(strcmp(argv[i],"-i") == 0) {
            
            finit = fopen(argv[i+1], "r");
            if (finit == NULL) {
                printf("Cannot open file %s\n", argv[i+1]);
                return 1;
            }
        }
        else if(strcmp(argv[i],"-q") == 0){
            
            fquery = fopen(argv[i+1], "r");
            if (fquery == NULL) {
                printf("Cannot open file %s\n", argv[i+1]);
                return 1;
            }
        }
        else {
            printf("Correct syntax is: %s -i <init_file> -q <query_file>\n", argv[0]);
            return 1;
        }
    }
 
    //Initializing Trie
    trieRoot = trieRootInit();
    //Input & Storing
    insert_Ngrams_In_Trie(finit, trieRoot);
        
    return 0;
}

int fpeek(FILE *fp){
    return ungetc(getc(fp),fp);
}

void insert_Ngrams_In_Trie(FILE* fp, TrieRoot* trieRoot){
    
    char buffer[WORD_SIZE];
    Ngramptr ngram;
    
    while(fpeek(fp) != EOF){
        
        ngram = Ngram_create();
        
        while(fpeek(fp) != '\n'){
            fscanf(fp, "%s", buffer);
            Ngram_Insert_End(ngram, buffer);
        }
        getc(fp);                                       //Consume new line character
        
        Ngram_print(ngram);
        Ngram_free(ngram);

        trieInsertSort(trieRoot, ngram);
    }
}