#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "bloomFilter.h"
#include "murmur.h"
#include "../errorMessages.h"


bool findInBloom(char *ngram, char *bloomfilter){
	unsigned int i, place, bit, value, hash, l;
	char prev;
	bool retval=true;

	l = strlen(ngram);
	for (i=0; i<K; i++){
		hash = (unsigned int) murmurhash(ngram, (uint32_t) l, i)%(BLOOMSIZE*8);
		place = (hash/8)%BLOOMSIZE;		    /* place has the place of the byte that has the desired bit */
		bit = hash%8;					    /* bit has the place of the desired bit in the byte */
		prev = bloomfilter[place];			/* prev has the value of the corresponding byte in the bloom filter BEFORE the change */
		value = 1;
		value = value << bit;				/* creating the new value of the desired bit */
		value = value | prev;				/* OR operation: value now has the new value of the byte that was to be changed */
		if ((value^prev) != 0)				/* if the XOR operation returns a non-zero value, it means the bit was changed (we have not encountered this ngram before) */
			retval = false;					/* if retval is false (at least one bit was changed), the ngram has not been encountered yet; print it */
		bloomfilter[place] = value;
	}
	return retval;
}
