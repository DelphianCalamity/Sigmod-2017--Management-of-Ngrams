#ifndef TOPK_HASHTABLE_H
#define	TOPK_HASHTABLE_H

#include "../trieStructs.h"

#define TOPK_IN_SIZE 5

typedef struct topK_hashtable_info TopK_Hashtable_Info;
typedef TopK_Hashtable_Info* TopK_Hashtable_Info_ptr;

typedef struct big_bucket Big_Bucket;
typedef struct topK_bucket TopK_Bucket;

struct topK_bucket {
	char* ngram;
	int appearances;
};

struct big_bucket {
	int max_size;
	int records;
	TopK_Bucket* topBuckets;
};

struct topK_hashtable_info {
	int p;
	int m;
	int round;
	int Records;                    //Number of records in the hashtable
	int Buckets;                	//size of hashtable - number of buckets
	float load_factor;
	Big_Bucket* Phashtable;      		//points to the hashtable
};

TopK_Hashtable_Info_ptr hashtable;

TopK_Hashtable_Info_ptr topK_Hashtable_create(int, float);
void topK_Hashtable_init_Big_bucket(Big_Bucket*);
void topK_Hashtable_init_bucket(TopK_Bucket*, char*);

void topK_Hashtable_Bucket_print(TopK_Hashtable_Info_ptr, Big_Bucket*);
void topK_Hashtable_print(TopK_Hashtable_Info_ptr);

double topK_Hashtable_hashkey(char*);
int topK_Hash_function(TopK_Hashtable_Info_ptr, double, int);

void topK_Hashtable_split(TopK_Hashtable_Info_ptr);
void topK_Hashtable_Check_LoadFactor(TopK_Hashtable_Info_ptr);
void topK_Hashtable_insert(TopK_Hashtable_Info_ptr, char*);
void topK_Hashtable_insert_child(TopK_Hashtable_Info_ptr, Big_Bucket*, char*);
void topK_Hashtable_move_bucket(TopK_Hashtable_Info_ptr, Big_Bucket*, TopK_Bucket*);

void topK_print_TopK(TopK_Hashtable_Info_ptr hashtable, int k);

void topK_Hashtable_Destroy(TopK_Hashtable_Info_ptr);

#endif


