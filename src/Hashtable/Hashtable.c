#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Hashtable.h"
#include "../trieStructs.h"
#include "../errorMessages.h"
#include "../BloomFilter/murmur.h"
#include <math.h>

/* *** FUNCTIONS *** */

Hashtable_Info_ptr Hashtable_create(int num_of_buckets, float lf) {

	int i;
	Hashtable_Info_ptr hashtable = safemalloc(sizeof(Hashtable_Info));
	hashtable->p = 0;
	hashtable->m = num_of_buckets;
	hashtable->round = 0;
	hashtable->Buckets = num_of_buckets;
	hashtable->Records = 0;
	hashtable->load_factor = lf;
	hashtable->Phashtable = safemalloc(sizeof(Bucket) * num_of_buckets);            	//Creates the table

	for (i = 0; i < num_of_buckets; i++)
		Hashtable_init_bucket(&hashtable->Phashtable[i]);

	return hashtable;
}

void Hashtable_init_bucket(Bucket* bucket) {
	bucket->emptySpace = IN_SIZE;
	bucket->maxChildren = IN_SIZE;
	bucket->deletedChildren = 0;
    bucket->children = safecalloc(IN_SIZE, sizeof(TrieNode));
	bucket->word = NULL;
	bucket->offsets = NULL;
	bucket->deleted=0;
}

/********************************************************/
void Hashtable_Bucket_print(Hashtable_Info_ptr hash, Bucketptr bucket) {

	int i;
	uint32_t key;
	for (i = 0; i < bucket->maxChildren-bucket->emptySpace; i++) {
		if (bucket->children[i].deleted == 0) {
			key = Hashtable_hashkey((bucket->children[i].word), strlen(bucket->children[i].word));
			printf("%s, %d, %d\n", (bucket->children[i].word), Hash_function(hash, key, hash->round), Hash_function(hash, key, hash->round + 1));
		}
	}
}

void Hashtable_print(Hashtable_Info_ptr hashtable) {

	int i;
	Bucketptr bucket = hashtable->Phashtable;

    printf("Hashtable information:\n    Round: %d\n    Initial number of buckets: %d\n    Bucket to be splitted: %d\n    Buckets: %d\n    Records: %d\n", \
            hashtable->round, hashtable->m, hashtable->p, hashtable->Buckets, hashtable->Records);

    for (i = 0; i < hashtable->Buckets; i++) {
        printf("--------------------------\n");
        Hashtable_Bucket_print(hashtable, bucket+i);
        printf("--------------------------\n\n\n");
    }
}

/********************************************************/
uint32_t Hashtable_hashkey(char* word, int len) {
	return murmurhash(word, (uint32_t) len, 0);
}

int Hash_function(Hashtable_Info_ptr hashtable, uint32_t key, int round) {       	// f(x) = k%(2^i)m
	int x;
	(round == 0) ? (x = hashtable->m) : (x = (2 << (round-1)) * hashtable->m);										//Initial number of buckets * 2^round*m
	return (int) (key % x);
}
/********************************************************/


TrieNode* Hashtable_insert(Hashtable_Info_ptr hashtable, BinaryResult* br, char* word, int len) {

	Bucketptr bucket = hashtable->Phashtable;
	uint32_t key = Hashtable_hashkey(word, len);
	int DestinationBucket = Hash_function(hashtable, key, hashtable->round);            		//Hash function of the current round
	if (DestinationBucket < hashtable->p) {
		DestinationBucket = Hash_function(hashtable, key, hashtable->round + 1);	            //Hash function of the next round
	}

	return Hashtable_insert_child(hashtable, br, bucket+DestinationBucket, word);
}


void Hashtable_Check_LoadFactor(Hashtable_Info_ptr hashtable) {

	/* If > Load_Factor Split */
	float fraction = (float) hashtable->Records / (hashtable->Buckets * IN_SIZE);              	//Without counting the overflow buckets
	if (fraction > hashtable->load_factor) {
		if (hashtable->p == 0)                                      							//First split of the round ---> Double the Hashtable by using realloc
			hashtable->Phashtable = saferealloc(hashtable->Phashtable, sizeof(Bucket) * hashtable->Buckets * 2);
		Hashtable_split(hashtable);
	}
}

TrieNode* Hashtable_insert_child(Hashtable_Info_ptr hashtable, BinaryResult* result, Bucketptr bucket, char* word) {

	trieMakeSpace(result, bucket, word);

	/*Store the new child and update children count*/
	if (result->found != 1) {                                          //If found != 1 the node must be inserted - otherwise it is already there
		trieNodeInit(word, &bucket->children[result->position]);
		if (result->found == 0)		                                   //If found == 2 the node replaced a deleted node
		{
			bucket->emptySpace--;
		}

		hashtable->Records++;
	}
	return &bucket->children[result->position];
}


void Hashtable_move_child(Hashtable_Info_ptr hashtable, Bucketptr bucket, TrieNode* node) {

	BinaryResult result;
	trieMakeSpace(&result, bucket, node->word);

	/*Store the new child and update children count*/
	memcpy(&bucket->children[result.position], node, sizeof(TrieNode));

	if (result.found == 0)
		bucket->emptySpace--;
}


/********************************************************/
void Hashtable_split(Hashtable_Info_ptr hashtable) {

	TrieNode* node;
	uint32_t key;
	int i, DestinationBucket, size;
    Bucketptr bucket = &(hashtable->Phashtable[hashtable->p]);

	Hashtable_init_bucket(hashtable->Phashtable + hashtable->Buckets);
    hashtable->Buckets++;

	size = bucket->maxChildren - bucket->emptySpace;
	for (i=0; i<size; i++) {

		node = &bucket->children[i];
		if (!node->deleted) {
			key = Hashtable_hashkey(node->word, strlen(node->word));
			DestinationBucket = Hash_function(hashtable, key, hashtable->round + 1);   							//REHASH

			if (DestinationBucket != hashtable->p) {
				Hashtable_move_child(hashtable, (hashtable->Phashtable + DestinationBucket), node);

				char* temp = node->word;																		//Copy the deleted node's word , cause an alive node points there too. (problem when delete frees word)
				node->word = safemalloc((strlen(temp)+1)*sizeof(char));
				strcpy(node->word, temp);
				node->children = NULL;
				node->deleted = 1;
				bucket->deletedChildren++;
			}
		}
	}

	i = bucket->maxChildren-bucket->emptySpace-1;

	while (i >= 0 && bucket->children[i].deleted) {				// free all deleted children after the last active one
		free(bucket->children[i].word);
		bucket->children[i].deleted = 0;
		bucket->deletedChildren--;
		bucket->emptySpace++;
		i--;
	}

	/*Creating space from deleted nodes*/
	if ((double) bucket->deletedChildren / (bucket->maxChildren - bucket->emptySpace) > FACTOR) {
		trieCompactSpace(bucket);
	}

	if ((hashtable->p + 1) == (pow(2, hashtable->round) * hashtable->m)) {          //If the buckets have been doubled during this round
		hashtable->round++;                 //Increase the round
		hashtable->p = 0;
	} else hashtable->p++;                  //Increase p
}


/********************************************************/

TrieNode* Hashtable_lookup(BinaryResult* result, Hashtable_Info_ptr hashtable, char* word, int len) {

	uint32_t key;
    Bucketptr bucket = hashtable->Phashtable;
	key = Hashtable_hashkey(word, len);

    int i = Hash_function(hashtable, key, hashtable->round);
    if (i < hashtable->p)
		i = Hash_function(hashtable, key, hashtable->round + 1);

	trieBinarySearch(result, bucket+i, word);

	if(result->found == 1)
		return bucket+i;

	return NULL;

}

TrieNode* Hashtable_lookup_Bucket(Hashtable_Info_ptr hashtable, char* word, int len) {

	uint32_t key;
	Bucketptr bucket = hashtable->Phashtable;
	key = Hashtable_hashkey(word, len);

	int i = Hash_function(hashtable, key, hashtable->round);
	if (i < hashtable->p)
		i = Hash_function(hashtable, key, hashtable->round + 1);

	return bucket+i;
}

/********************************************************/

void Hashtable_Destroy(Hashtable_Info_ptr hashtable) {
    free(hashtable->Phashtable);
    free(hashtable);
}
