#ifndef HASHTABLE_H
#define	HASHTABLE_H

#include "../trieStructs.h"

#define IN_SIZE 5

typedef struct trieNode TrieNode;				//Forward Declaration
typedef struct binaryResult BinaryResult;		//Forward Declaration

typedef TrieNode Bucket;
typedef Bucket* Bucketptr;

typedef struct hashtable_info Hashtable_Info;
typedef Hashtable_Info* Hashtable_Info_ptr;

struct hashtable_info {
	int p;
	int m;
	int round;
	int Records;                    //Number of records in the hashtable
	int Buckets;                	//size of hashtable - number of buckets
	float load_factor;
	Bucketptr Phashtable;      		//points to the hashtable
};

Hashtable_Info_ptr Hashtable_create(int, float);
void Hashtable_init_bucket(Bucket*);

void Hashtable_Bucket_print(Hashtable_Info_ptr, Bucketptr);
void Hashtable_print(Hashtable_Info_ptr);

double Hashtable_hashkey(char*);
int Hash_function(Hashtable_Info_ptr, char*, int);

TrieNode* Hashtable_insert(Hashtable_Info_ptr, BinaryResult*, char*);
void Hashtable_Check_LoadFactor(Hashtable_Info_ptr);
TrieNode* Hashtable_insert_child(Hashtable_Info_ptr, BinaryResult*, Bucketptr, char*);
void Hashtable_move_child(Hashtable_Info_ptr, Bucketptr, TrieNode*);
void Hashtable_split(Hashtable_Info_ptr);

TrieNode* Hashtable_lookup(BinaryResult*, Hashtable_Info_ptr, char*);
TrieNode* Hashtable_lookup_Bucket(Hashtable_Info_ptr, char*);

void Hashtable_Destroy(Hashtable_Info_ptr hashtable);

#endif


