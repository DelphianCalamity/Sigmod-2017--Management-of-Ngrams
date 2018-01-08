#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "unit_testing.h"
#include "../BloomFilter/bloomFilter.h"
#include "../BloomFilter/murmur.h"
#include "../CompactTrie/compactTree.h"
#include "../Hashtable/Hashtable.h"
#include "../Threads/JobScheduler.h"
#include "../TopK/topK_Hashtable.h"
#include "../bursts.h"
#include "../errorMessages.h"
#include "../ngram.h"
#include "../parsing.h"
#include "../trieStructs.h"


void testBinarySearch() {

	//some testing of binary search
	BinaryResult br;
	TrieNode node;

	trieNodeInit(NULL, &node);

	trieNodeInit("aa", &node.children[0]);
	node.emptySpace--;
	trieNodeInit("ab", &node.children[1]);
	node.emptySpace--;
	trieNodeInit("boo", &node.children[2]);
	node.emptySpace--;

	trieBinarySearch(&br, &node, "b");
	printf("found=%d\nposition=%d\n", br.found, br.position);
	assert(br.found == 0 && br.position == 2);

	trieBinarySearch(&br, &node, "a");
	printf("found=%d\nposition=%d\n", br.found, br.position);
	assert(br.found == 0 && br.position == 0);

	trieBinarySearch(&br, &node, "c");
	printf("found=%d\nposition=%d\n", br.found, br.position);
	assert(br.found == 0 && br.position == 3);

	trieBinarySearch(&br, &node, "aa");
	printf("found=%d\nposition=%d\n", br.found, br.position);
	assert(br.found == 1 && br.position == 0);

	free(node.children[0].children);
	free(node.children[1].children);
	free(node.children[2].children);
	free(node.children);

	printf("Binary Search test, completed successfully\n");
}


void confirmExistence(NgramVector *ngram) {

	int i;
	BinaryResult br;
	TrieNode *node;

	/*Find first word's hash*/
	int hashKey = (murmurhash(ngram->ngram[0], strlen(ngram->ngram[0]), 0)) % trieRoot->hashtable->m;

	/*Find correct bucket*/
	node = &trieRoot->hashtable->Phashtable[hashKey];

	/*Search the bucket and go down the trie*/
	for (i = 0; i < ngram->words; i++) {                //Confirm each word's existence in Trie
		trieBinarySearch(&br, node, ngram->ngram[i]);
		assert(br.found == 1);
		/*Move to next node */
		node = &node->children[br.position];
	}
}


void testInsert() {

	/*Initialise a test trie*/
	trieRootInit();

	char *str;
	NgramVector *ngram;

	/*-----------Test 1--------------*/

	str = "this is a dog";
	ngram = testCreateNgram(str, strlen(str) + 1);
	trieInsertSort(ngram);
	deleteWords(ngram);
	deleteNgram(ngram);

	ngram = testCreateNgram(str, strlen(str) + 1);
	confirmExistence(ngram);
	deleteWords(ngram);
	deleteNgram(ngram);

	/*-----------Test 2--------------*/

	str = "this is a cat";
	ngram = testCreateNgram(str, strlen(str) + 1);
	trieInsertSort(ngram);
	deleteWords(ngram);
	deleteNgram(ngram);

	ngram = testCreateNgram(str, strlen(str) + 1);
	confirmExistence(ngram);
	deleteWords(ngram);
	deleteNgram(ngram);

	/*-----------Test 3--------------*/

	str = "the fox is bad";
	ngram = testCreateNgram(str, strlen(str) + 1);
	trieInsertSort(ngram);
	deleteWords(ngram);
	deleteNgram(ngram);

	ngram = testCreateNgram(str, strlen(str) + 1);
	confirmExistence(ngram);
	deleteWords(ngram);
	deleteNgram(ngram);

	/*-----------Test 4--------------*/

	str = "the car is red";
	ngram = testCreateNgram(str, strlen(str) + 1);
	trieInsertSort(ngram);
	deleteWords(ngram);
	deleteNgram(ngram);

	ngram = testCreateNgram(str, strlen(str) + 1);
	confirmExistence(ngram);
	deleteWords(ngram);
	deleteNgram(ngram);

	/*-----------Test 5--------------*/

	str = "this is a test";
	ngram = testCreateNgram(str, strlen(str) + 1);
	trieInsertSort(ngram);
	deleteWords(ngram);
	deleteNgram(ngram);

	ngram = testCreateNgram(str, strlen(str) + 1);
	confirmExistence(ngram);
	deleteWords(ngram);
	deleteNgram(ngram);

	/*-----------Test 6--------------*/

	str = "this is a";
	ngram = testCreateNgram(str, strlen(str) + 1);
	trieInsertSort(ngram);
	deleteWords(ngram);
	deleteNgram(ngram);

	ngram = testCreateNgram(str, strlen(str) + 1);
	confirmExistence(ngram);
	deleteWords(ngram);
	deleteNgram(ngram);

	/*-------------------------*/

	/*Free test trie*/
	trieFree();

	printf("Insert test complete successfully\n");
}


void testCreateTestCase() {

	char *str;
	NgramVector *ngram;

	str = "this is a dog";
	ngram = testCreateNgram(str, strlen(str) + 1);
	trieInsertSort(ngram);
	deleteWords(ngram);
	deleteNgram(ngram);

	str = "this is a cat";
	ngram = testCreateNgram(str, strlen(str) + 1);
	trieInsertSort(ngram);
	deleteWords(ngram);
	deleteNgram(ngram);

	str = "the fox is bad";
	ngram = testCreateNgram(str, strlen(str) + 1);
	trieInsertSort(ngram);
	deleteWords(ngram);
	deleteNgram(ngram);

	str = "the car is red";
	ngram = testCreateNgram(str, strlen(str) + 1);
	trieInsertSort(ngram);
	deleteWords(ngram);
	deleteNgram(ngram);

	str = "this is a test";
	ngram = testCreateNgram(str, strlen(str) + 1);
	trieInsertSort(ngram);
	deleteWords(ngram);
	deleteNgram(ngram);

	str = "this is a";
	ngram = testCreateNgram(str, strlen(str) + 1);
	trieInsertSort(ngram);
	deleteWords(ngram);
	deleteNgram(ngram);
}

int checkSearch(NgramVector *ngram) {

	int i, x = trieRoot->hashtable->m, *bloomfilter;
	char *buffer = safemalloc(100 * sizeof(char));



	/*Initialise a bloomfilter*/
	bloomfilter = safemalloc(100);

	TrieNode *root = &trieRoot->hashtable->Phashtable[(murmurhash(ngram->ngram[0], strlen(ngram->ngram[0]), 0)) % x];
	int check = 0;
	for (i = 0; i < ngram->words; i++)                                         //For all root's children
//		trieSearch_Ngram(root, 0, 0, ngram, buffer, 100, 0, bloomfilter);
//
		return 0;
}


void testSearch() {
	char *str, *bloomfilter;
	NgramVector *ngram;
	int check;

	/*initialize a test trie*/
	trieRootInit();
	testCreateTestCase();

	/*Create test ngram*/
	str = "the car is red and blue and this is a test";
	ngram = testCreateNgram(str, strlen(str) + 1);

	/*Test search with test-ngram*/
	check = checkSearch(ngram);
	assert(check == 3);

	/*Delete test ngram*/
	deleteWords(ngram);
	deleteNgram(ngram);

	/*Delete test trie */
	trieFree();

	printf("End Of Search testing..\n");
}


void testDelete() {

	trieRootInit();

	int check;
	char *str;
	NgramVector *ngram;

	testCreatTestCase();

	str = "this is a dog";
	ngram = testCreateNgram(str, strlen(str) + 1);
	trieDeleteNgram(ngram);
	deleteWords(ngram);
	deleteNgram(ngram);
	ngram = testCreateNgram(str, strlen(str) + 1);
	check = checkSearch(ngram);
	assert(check == 1);
	deleteWords(ngram);
	deleteNgram(ngram);

	str = "this is a cat";
	ngram = testCreateNgram(str, strlen(str) + 1);
	trieDeleteNgram(ngram);
	deleteWords(ngram);
	deleteNgram(ngram);
	ngram = testCreateNgram(str, strlen(str) + 1);
	check = checkSearch(ngram);
	assert(check == 1);
	deleteWords(ngram);
	deleteNgram(ngram);

	str = "the fox is bad";
	ngram = testCreateNgram(str, strlen(str) + 1);
	trieDeleteNgram(ngram);
	deleteWords(ngram);
	deleteNgram(ngram);
	ngram = testCreateNgram(str, strlen(str) + 1);
	check = checkSearch(ngram);
	assert(check == 0);
	deleteWords(ngram);
	deleteNgram(ngram);

	str = "the car is red";
	ngram = testCreateNgram(str, strlen(str) + 1);
	trieDeleteNgram(ngram);
	deleteWords(ngram);
	deleteNgram(ngram);
	ngram = testCreateNgram(str, strlen(str) + 1);
	check = checkSearch(ngram);
	assert(check == 0);
	deleteWords(ngram);
	deleteNgram(ngram);

	str = "this is a test";
	ngram = testCreateNgram(str, strlen(str) + 1);
	trieDeleteNgram(ngram);
	deleteWords(ngram);
	deleteNgram(ngram);
	ngram = testCreateNgram(str, strlen(str) + 1);
	check = checkSearch(ngram);
	assert(check == 1);
	deleteWords(ngram);
	deleteNgram(ngram);

	str = "this is a";
	ngram = testCreateNgram(str, strlen(str) + 1);
	trieDeleteNgram(ngram);
	deleteWords(ngram);
	deleteNgram(ngram);
	ngram = testCreateNgram(str, strlen(str) + 1);
	check = checkSearch(ngram);
	assert(check == 0);
	deleteWords(ngram);
	deleteNgram(ngram);

	trieFree();

	printf("End Of Delete testing..\n");
}

NgramVector *testCreateNgram(char *buffer, ssize_t size) {

	NgramVector *ngramVector = initNgram();
	int s = 0, e;

	ngramVector->words = 0;

	for (e = 0; e < size; e++) {
		if (!(buffer[e] != ' ' && buffer[e] != '\0')) {

			if (s != e) {
				if (ngramVector->words == ngramVector->capacity) {
					ngramVector->capacity *= 2;
					ngramVector->ngram = saferealloc(ngramVector->ngram, ngramVector->capacity * sizeof(char *));

				}

				ngramVector->ngram[ngramVector->words] = safemalloc((e - s + 1) * sizeof(char));
				memcpy(ngramVector->ngram[ngramVector->words], &buffer[s], e - s);
				ngramVector->ngram[ngramVector->words][e - s] = '\0';
				ngramVector->words++;
			}
			s = e + 1;
		}
	}

	return ngramVector;
}

void ConfirmCompactExistence(NgramVector *ngram, char * firstWord){
	int i;
	BinaryResult br;
	TrieNode *node;

	/*Find first word's hash*/
	int hashKey = (murmurhash(firstWord, strlen(firstWord), 0)) % trieRoot->hashtable->m;

	/*Find correct bucket*/
	node = &trieRoot->hashtable->Phashtable[hashKey];

	/*Search the bucket and go down the trie*/
	for (i = 0; i < ngram->words; i++) {                //Confirm each word's existence in Trie
		trieBinarySearch(&br, node, ngram->ngram[i]);
		assert(br.found == 1);
		/*Move to next node */
		node = &node->children[br.position];
	}
}

void TestCompactTree(){
	trieRootInit();
	testCreateTestCase();
	trieCompactTree();
	ConfirmCompactExistence(testCreateNgram("thisis a dog", strlen("thisis a dog")), "this");

}