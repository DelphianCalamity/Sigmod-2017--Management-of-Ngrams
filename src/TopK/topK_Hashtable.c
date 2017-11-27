#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "topK_Hashtable.h"
//#include "../trieStructs.h"
#include "../errorMessages.h"
#include <math.h>

/* *** FUNCTIONS *** */

TopK_Hashtable_Info_ptr topK_Hashtable_create(int big_buckets, float lf) {

	int i;
	TopK_Hashtable_Info_ptr hashtable = safemalloc(sizeof(TopK_Hashtable_Info));
	hashtable->p = 0;
	hashtable->m = big_buckets;
	hashtable->round = 0;
	hashtable->Buckets = big_buckets;
	hashtable->Records = 0;
	hashtable->load_factor = lf;
	hashtable->Phashtable = safemalloc(sizeof(Big_Bucket) * big_buckets);            	//Creates the table

	for (i = 0; i < big_buckets; i++)
		topK_Hashtable_init_Big_bucket(&hashtable->Phashtable[i]);

	return hashtable;
}

void topK_Hashtable_init_Big_bucket(Big_Bucket* bucket) {
	bucket->records = 0;
	bucket->max_size = TOPK_IN_SIZE;
    bucket->topBuckets = safecalloc(TOPK_IN_SIZE, sizeof(TopK_Bucket));
}

void topK_Hashtable_init_bucket(TopK_Bucket* bucket, char* ngram) {
	bucket->appearances = 1;
	bucket->ngram = safemalloc(strlen(ngram) * sizeof(TopK_Bucket));
	strcpy(bucket->ngram, ngram);
}

/********************************************************/
void topK_Hashtable_Bucket_print(TopK_Hashtable_Info_ptr hash, Big_Bucket *bucket) {

	int i;
	double key;
	for (i = 0; i < bucket->records; i++) {
			key = topK_Hashtable_hashkey((bucket->topBuckets[i].ngram));
			printf("%s, %d, %d\n", bucket->topBuckets[i].ngram, topK_Hash_function(hash, key, hash->round), topK_Hash_function(hash, key, hash->round + 1));
	}
}

void topK_Hashtable_print(TopK_Hashtable_Info_ptr hashtable) {

	int i;
	Big_Bucket *bucket = hashtable->Phashtable;

    printf("Hashtable information:\n    Round: %d\n    Initial number of buckets: %d\n    Bucket to be splitted: %d\n    Buckets: %d\n    Records: %d\n", \
            hashtable->round, hashtable->m, hashtable->p, hashtable->Buckets, hashtable->Records);

    for (i = 0; i < hashtable->Buckets; i++) {
        printf("--------------------------\n");
		topK_Hashtable_Bucket_print(hashtable, bucket+i);
        printf("--------------------------\n\n\n");
    }
}

/********************************************************/

double topK_Hashtable_hashkey(char* ngram) {

	double key = 0;
	int i, len = strlen(ngram);

	for(i=0; i < len; i++)
		key += (int)ngram[i];
	return key;
}

int topK_Hash_function(TopK_Hashtable_Info_ptr hashtable, double key, int round) {       			// f(x) = k%(2^i)m
	int x = pow(2, round) * hashtable->m;															//Initial number of buckets * 2^round*m
	return (int) key % x;
}
/********************************************************/


void topK_Hashtable_insert(TopK_Hashtable_Info_ptr hashtable, char* ngram) {

	Big_Bucket* bucket = hashtable->Phashtable;
	double key = topK_Hashtable_hashkey(ngram);
	int DestinationBucket = topK_Hash_function(hashtable, key, hashtable->round);            		//Hash function of the current round
	if (DestinationBucket < hashtable->p)
		DestinationBucket = topK_Hash_function(hashtable, key, hashtable->round + 1);        		//Hash function of the next round

	return topK_Hashtable_insert_child(hashtable, bucket+DestinationBucket, ngram);
}


void topK_Hashtable_Check_LoadFactor(TopK_Hashtable_Info_ptr hashtable) {

	/* If > Load_Factor Split */
	float fraction = (float) hashtable->Records / (hashtable->Buckets * TOPK_IN_SIZE);           	//Without counting the overflow buckets
	if (fraction > hashtable->load_factor) {
		if (hashtable->p == 0)                                      								//First split of the round ---> Double the Hashtable by using realloc
			hashtable->Phashtable = saferealloc(hashtable->Phashtable, sizeof(Big_Bucket) * hashtable->Buckets * 2);
		topK_Hashtable_split(hashtable);
	}
}

void topK_Hashtable_insert_child(TopK_Hashtable_Info_ptr hashtable, Big_Bucket* big_bucket, char* ngram) {

	TopK_Bucket temp;
	int i=0, appearances, position;
	TopK_Bucket* buckets = big_bucket->topBuckets;

	for(i=0; i<big_bucket->records; i++){

		//If such ngram is found - update its appearances
		if(strcmp(buckets[i].ngram, ngram) == 0) {
			position = i;
			buckets[i].appearances++;
			appearances = buckets[i].appearances;

			//Determine bucket's new position
			while (i > 0 && appearances > buckets[i - 1].appearances)
				i--;

			while (i > 0 &&  appearances == buckets[i - 1].appearances) {

				if(strcmp(ngram, buckets[i - 1].ngram) < 0)				//If it has priority
					i--;
			}

			if(position != i){											//swap i, position
				memcpy(&temp, &buckets[position], sizeof(TopK_Bucket));
				memmove(buckets + i + 1, buckets + i, (position-i) * sizeof(TopK_Bucket));
				memcpy(buckets + i, &temp, sizeof(TopK_Bucket));
			}
		}
			//If ngram is not found - insert it in the right position
		else {

			if(big_bucket->records == big_bucket->max_size){
				big_bucket->topBuckets = saferealloc(big_bucket->topBuckets, (big_bucket->max_size* 2) * sizeof(TopK_Bucket));
				big_bucket->max_size *= 2;
			}

			i = position = big_bucket->records;

			while (i > 0 &&  buckets[i-1].appearances == 1) {
				if(strcmp(ngram, buckets[i-1].ngram) < 0)				//If it has priority
					i--;
			}

			memmove(buckets + i + 1, buckets + i, (position-i) * sizeof(TopK_Bucket));
			topK_Hashtable_init_bucket(buckets+i, ngram);

			big_bucket->records++;
			hashtable->Records++;
		}
	}
}

void topK_Hashtable_move_bucket(TopK_Hashtable_Info_ptr hashtable, Big_Bucket* big_bucket, TopK_Bucket* bucket) {

	int i, position, appearances;
	TopK_Bucket* buckets = big_bucket->topBuckets;

	if(big_bucket->records == big_bucket->max_size){
		big_bucket->topBuckets = saferealloc(big_bucket->topBuckets, (big_bucket->max_size* 2) * sizeof(TopK_Bucket));
		big_bucket->max_size *= 2;
	}

	i = position = big_bucket->records;
	appearances = bucket->appearances;

	//Determine bucket's new position
	while (i > 0 && appearances > buckets[i-1].appearances)
		i--;

	while (i > 0 &&  appearances == buckets[i-1].appearances) {

		if(strcmp(bucket->ngram, buckets[i-1].ngram) < 0)				//If it has priority
			i--;
	}
	memmove(buckets + i + 1, buckets + i, (position-i) * sizeof(TopK_Bucket));
	memcpy(buckets + i, bucket, sizeof(TopK_Bucket));

	big_bucket->records++;
}


/********************************************************/
void topK_Hashtable_split(TopK_Hashtable_Info_ptr hashtable) {

	double key;
	TopK_Bucket* bucket;
	int i, DestinationBucket;
    Big_Bucket* big_bucket = &hashtable->Phashtable[hashtable->p];

	topK_Hashtable_init_Big_bucket(hashtable->Phashtable + hashtable->Buckets);
    hashtable->Buckets++;

	for (i=0; i<big_bucket->records; i++) {

		bucket = &big_bucket->topBuckets[i];

		key = topK_Hashtable_hashkey(bucket->ngram);
		DestinationBucket = topK_Hash_function(hashtable, key, hashtable->round + 1);   							//REHASH

		if (DestinationBucket != hashtable->p) {

			topK_Hashtable_move_bucket(hashtable, (hashtable->Phashtable + DestinationBucket), bucket);
			bucket->ngram = NULL;
			big_bucket->records--;
		}
	}

    if ((hashtable->p + 1) == (pow(2, hashtable->round) * hashtable->m)) {          //If the buckets have been doubled during this round
        hashtable->round++;                 //Increase the round
        hashtable->p = 0;
    } else hashtable->p++;                  //Increase p
}


/********************************************************/
void topK_Hashtable_Destroy(TopK_Hashtable_Info_ptr hashtable) {

	int i,j;
	Big_Bucket* big_bucket;

	for(i=0; i<hashtable->Buckets; i++) {

		big_bucket = &hashtable->Phashtable[i];
		for(j=0; j < big_bucket->records; j++)
			free(big_bucket->topBuckets[j].ngram);
	}
	free(hashtable->Phashtable);
    free(hashtable);
}
