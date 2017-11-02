#include <stdio.h>
#include "unit_testing.h"
#include "trieStructs.h"
#include <assert.h>
#include "ngram.h"
#include "errorMessages.h"
#include "trieStructs.h"
#include <stdlib.h>
#include <string.h>

void testBinarySearch() {

    //some testing of binary search
    BinaryResult br;
    TrieNode node;

    trieNodeInit("", &node);

    trieNodeInit("aa", &node.children[0]);
    node.emptySpace--;
    trieNodeInit("ab", &node.children[1]);
    node.emptySpace--;
    trieNodeInit("boo", &node.children[2]);
    node.emptySpace--;

    trieBinarySearch(&br, &node, "b");
    printf("found=%d\nposition=%d\n", br.found, br.position);
    assert(br.found==0 && br.position==2);

    trieBinarySearch(&br, &node, "a");
    printf("found=%d\nposition=%d\n", br.found, br.position);
    assert(br.found==0 && br.position==0);

    trieBinarySearch(&br, &node, "c");
    printf("found=%d\nposition=%d\n", br.found, br.position);
    assert(br.found==0 && br.position==3);

    trieBinarySearch(&br, &node, "aa");
    printf("found=%d\nposition=%d\n", br.found, br.position);
    assert(br.found==1 && br.position==0);

    printf("End Of Binary Search testing..\n");
}

void confirmExistence(NgramVector* ngram){

    int i;
    BinaryResult br;
    TrieNode* node = trieRoot->root;

    for (i = 0; i < ngram->words; i++){                //Confirm each word's existence in Trie
        trieBinarySearch(&br, node, ngram->ngram[i]);
        assert(br.found==1);

        node = &node->children[br.position];
    }
}


void testInsert() {

    trieRootInit();

    char* str;
    NgramVector* ngram;

    str = "this is a dog";
    ngram = testCreateNgram(str, strlen(str)+1);
    trieInsertSort(ngram);
    deleteWords(ngram);
    deleteNgram(ngram);
    ngram = testCreateNgram(str, strlen(str)+1);
    confirmExistence(ngram);
    deleteWords(ngram);
    deleteNgram(ngram);

    str = "this is a cat";
    ngram = testCreateNgram(str, strlen(str)+1);
    trieInsertSort(ngram);
    deleteWords(ngram);
    deleteNgram(ngram);
    ngram = testCreateNgram(str, strlen(str)+1);
    confirmExistence(ngram);
    deleteWords(ngram);
    deleteNgram(ngram);

    str = "the fox is bad";
    ngram = testCreateNgram(str, strlen(str)+1);
    trieInsertSort(ngram);
    deleteWords(ngram);
    deleteNgram(ngram);
    ngram = testCreateNgram(str, strlen(str)+1);
    confirmExistence(ngram);
    deleteWords(ngram);
    deleteNgram(ngram);


    str = "the car is red";
    ngram = testCreateNgram(str, strlen(str)+1);
    trieInsertSort(ngram);
    deleteWords(ngram);
    deleteNgram(ngram);
    ngram = testCreateNgram(str, strlen(str)+1);
    confirmExistence(ngram);
    deleteWords(ngram);
    deleteNgram(ngram);


    str = "this is a test";
    ngram = testCreateNgram(str, strlen(str)+1);
    trieInsertSort(ngram);
    deleteWords(ngram);
    deleteNgram(ngram);
    ngram = testCreateNgram(str, strlen(str)+1);
    confirmExistence(ngram);
    deleteWords(ngram);
    deleteNgram(ngram);


    str = "this is a";
    ngram = testCreateNgram(str, strlen(str)+1);
    trieInsertSort(ngram);
    deleteWords(ngram);
    deleteNgram(ngram);
    ngram = testCreateNgram(str, strlen(str)+1);
    confirmExistence(ngram);
    deleteWords(ngram);
    deleteNgram(ngram);

    trieFree();

    printf("End Of insert testing..\n");
}


void testSearch(){

    trieRootInit();

    char* str;
    NgramVector* ngram;

    testCreatTestCase();

    str = "the car is red and blue and this is a test";
    ngram = testCreateNgram(str, strlen(str)+1);

    int check;

    check = checkSearch(ngram);
    assert(check==3);

    deleteWords(ngram);
    deleteNgram(ngram);

    trieFree();

    printf("End Of Search testing..\n");
}



void testDelete() {

    trieRootInit();

    int check;
    char* str;
    NgramVector* ngram;

    testCreatTestCase();

    str = "this is a dog";
    ngram = testCreateNgram(str, strlen(str)+1);
    trieDeleteNgram(ngram);
    deleteWords(ngram);
    deleteNgram(ngram);
    ngram = testCreateNgram(str, strlen(str)+1);
    check = checkSearch(ngram);
    assert(check==1);
    deleteWords(ngram);
    deleteNgram(ngram);

    str = "this is a cat";
    ngram = testCreateNgram(str, strlen(str)+1);
    trieDeleteNgram(ngram);
    deleteWords(ngram);
    deleteNgram(ngram);
    ngram = testCreateNgram(str, strlen(str)+1);
    check = checkSearch(ngram);
    assert(check==1);
    deleteWords(ngram);
    deleteNgram(ngram);

    str = "the fox is bad";
    ngram = testCreateNgram(str, strlen(str)+1);
    trieDeleteNgram(ngram);
    deleteWords(ngram);
    deleteNgram(ngram);
    ngram = testCreateNgram(str, strlen(str)+1);
    check = checkSearch(ngram);
    assert(check==0);
    deleteWords(ngram);
    deleteNgram(ngram);

    str = "the car is red";
    ngram = testCreateNgram(str, strlen(str)+1);
    trieDeleteNgram(ngram);
    deleteWords(ngram);
    deleteNgram(ngram);
    ngram = testCreateNgram(str, strlen(str)+1);
    check = checkSearch(ngram);
    assert(check==0);
    deleteWords(ngram);
    deleteNgram(ngram);

    str = "this is a test";
    ngram = testCreateNgram(str, strlen(str)+1);
    trieDeleteNgram(ngram);
    deleteWords(ngram);
    deleteNgram(ngram);
    ngram = testCreateNgram(str, strlen(str)+1);
    check = checkSearch(ngram);
    assert(check==1);
    deleteWords(ngram);
    deleteNgram(ngram);

    str = "this is a";
    ngram = testCreateNgram(str, strlen(str)+1);
    ngram = testCreateNgram(str, strlen(str)+1);
    trieDeleteNgram(ngram);
    deleteWords(ngram);
    deleteNgram(ngram);
    ngram = testCreateNgram(str, strlen(str)+1);
    check = checkSearch(ngram);
    assert(check==0);
    deleteWords(ngram);
    deleteNgram(ngram);

    trieFree();

    printf("End Of Delete testing..\n");
}


int checkSearch(NgramVector* ngram){

    TrieNode *root = trieRoot->root;
    int i, check=0;
    for (i = 0; i < ngram->words; i++)                                         //For all root's children
        trieSearch_Ngram(root, i, i, ngram, &check);

    if (check > 0)
        trieRoot->lastQuery++;
    else printf("-1");
    printf("\n");

   return check;
}

void testCreatTestCase(){

    char* str;
    NgramVector* ngram;

    str = "this is a dog";
    ngram = testCreateNgram(str, strlen(str)+1);
    trieInsertSort(ngram);
    deleteWords(ngram);
    deleteNgram(ngram);

    str = "this is a cat";
    ngram = testCreateNgram(str, strlen(str)+1);
    trieInsertSort(ngram);
    deleteWords(ngram);
    deleteNgram(ngram);

    str = "the fox is bad";
    ngram = testCreateNgram(str, strlen(str)+1);
    trieInsertSort(ngram);
    deleteWords(ngram);
    deleteNgram(ngram);

    str = "the car is red";
    ngram = testCreateNgram(str, strlen(str)+1);
    trieInsertSort(ngram);
    deleteWords(ngram);
    deleteNgram(ngram);

    str = "this is a test";
    ngram = testCreateNgram(str, strlen(str)+1);
    trieInsertSort(ngram);
    deleteWords(ngram);
    deleteNgram(ngram);

    str = "this is a";
    ngram = testCreateNgram(str, strlen(str)+1);
    trieInsertSort(ngram);
    deleteWords(ngram);
    deleteNgram(ngram);
}


NgramVector* testCreateNgram(char *buffer, ssize_t size) {

    NgramVector *ngramVector = initNgram();
    int s=0, e;

    ngramVector->words = 0;

    for (e=0; e<size; e++) {
        if (!(buffer[e] != ' ' && buffer[e] != '\0')) {

            if (s != e){
                if (ngramVector->words == ngramVector->capacity){
                    ngramVector->capacity *= 2;
                    ngramVector->ngram = saferealloc(ngramVector->ngram, ngramVector->capacity*sizeof(char*));

                }

                ngramVector->ngram[ngramVector->words] = safemalloc((e-s+1)*sizeof(char));
                memcpy(ngramVector->ngram[ngramVector->words], &buffer[s], e-s);
                ngramVector->ngram[ngramVector->words][e-s]='\0';
                ngramVector->words++;
            }
            s = e+1;
        }
    }

    return ngramVector;
}