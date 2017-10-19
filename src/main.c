#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "trieStructs.h"
#include "parsing.h"
#include "errorMessages.h"
#include "bursts.h"
#include <unistd.h>

//NgramVector* read(char *inputFile);

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

    trieRootInit();                     //Initializing Trie
    readInputFile(finit);             //Input & Storing
    readQueryFile(fquery);
    // Burst *temp = burstListStart->start;
    // //getchar();
    // while(temp != NULL){
    //     //printf("%s     %c ",temp->next->ngram->ngram[1], temp->command);
    //     getchar();
    //     for (i=0; i<temp->ngram->words-1; i++){
    //                     getchar();
    //         printf("%s ", temp->ngram->ngram[i]);

    //     }
    //     printf("\n");
    //     temp = temp->next;
    // }
    executeBurstCommands(burstListStart);
    //readQueryFile(fquery);

    //NgramVector* ngram = read("search.txt");

    
    /*NgramVector ngram1;
    ngram1.ngram = malloc(4*sizeof(char*));
    ngram1.ngram[0] = "this";
    ngram1.ngram[1] = "is";
    ngram1.ngram[2] = "a";
    ngram1.ngram[3] = "dog";
    ngram1.words=4;
    trieInsertSort(&ngram1);


    NgramVector ngram2;
    ngram2.ngram = malloc(4*sizeof(char*));
    ngram2.ngram[0] = "this";
    ngram2.ngram[1] = "is";
    ngram2.ngram[2] = "a";
    ngram2.ngram[3] = "cat";
    ngram2.words=4;
    trieInsertSort(&ngram2);

    NgramVector ngram3;
    ngram3.ngram = malloc(4*sizeof(char*));
    ngram3.ngram[0] = "the";
    ngram3.ngram[1] = "fox";
    ngram3.ngram[2] = "is";
    ngram3.ngram[3] = "bad";
    ngram3.words=4;
    trieInsertSort(&ngram3);

    NgramVector ngram4;
    ngram4.ngram = malloc(4*sizeof(char*));
    ngram4.ngram[0] = "the";
    ngram4.ngram[1] = "car";
    ngram4.ngram[2] = "is";
    ngram4.ngram[3] = "red";
    ngram4.words=4;
    trieInsertSort(&ngram4);


    NgramVector ngram5;
    ngram5.ngram = malloc(4*sizeof(char*));
    ngram5.ngram[0] = "this";
    ngram5.ngram[1] = "is";
    ngram5.ngram[2] = "the";
    ngram5.ngram[3] = "test";
    ngram5.words=4;
    trieInsertSort(&ngram5);

    NgramVector ngram6;
    ngram6.ngram = malloc(3*sizeof(char*));
    ngram6.ngram[0] = "this";
    ngram6.ngram[1] = "is";
    ngram6.ngram[2] = "a";
    ngram6.words=3;
    trieInsertSort(&ngram6);



    NgramVector ngram;
    ngram.ngram = malloc(11*sizeof(char*));
    ngram.ngram[0] = "the";
    ngram.ngram[1] = "car";
    ngram.ngram[2] = "is";
    ngram.ngram[3] = "red";
    ngram.ngram[4] = "and";
    ngram.ngram[5] = "blue";
    ngram.ngram[6] = "and";
    ngram.ngram[7] = "this";
    ngram.ngram[8] = "is";
    ngram.ngram[9] = "the";
    ngram.ngram[10] = "test";

    ngram.words=11;

    char* buffer = trieSearch(&ngram);
    fflush(stdout);
    printf("%s\n", buffer);

    trieDeleteNgram(&ngram6);
    trieDeleteNgram(&ngram5);

    buffer = trieSearch(&ngram);
    printf("%s\n", buffer);
    free(buffer);*/
    //trieFree();

    return 0;
}

/*
NgramVector* read(char *inputFile){              //FOR TESTING SEARCH
    int i;
    char buffer[1024], stop=0;
    NgramVector *ngram;
    FILE *fp;

    if ((fp = fopen(inputFile, "r")) == NULL){				// open graph file
        getError(3);
        exit(3);
    }

    ngram = initNgram();
    while (fgets(buffer, 1024, fp) != NULL){				// while there are still uninserted n-grams
        i = strlen(buffer);
        if (buffer[i-1] == '\n'){							// remove \n for easier handling
            buffer[i-1] = '\0';
        }
        createNgram(ngram, buffer);
    }
    fclose(fp);

    return ngram;
}
 */