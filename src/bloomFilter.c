#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "bloomFilter.h"
#include "errorMessages.h"
#include "murmur.h"

void initBloom(void){
	bloomfilter = safemalloc(BLOOMSIZE);
	cells = safemalloc(K);
}

unsigned long hash(char *ngram, unsigned int h){
    unsigned long hash = 5381;
    int c, i;

	i=0;
    while (c = ngram[i++])
	    hash = (((hash << h) + hash) + c);
    return hash;
}

bool findInBloom(char *ngram){
	unsigned int i, place, bit, value;
	char prev;
	bool retval=true;

	memset(cells, 0, K*sizeof(unsigned int));

	for (i=0; i<K; i++){
		//cells[i] = hash(ngram, i)%(BLOOMSIZE*8);
		cells[i] = (int) murmurhash(ngram, (uint32_t) strlen(ngram), i);
		place = (cells[i]/8)%BLOOMSIZE;		/* place has the place of the byte that has the desired bit */
		bit = cells[i]%8;					/* bit has the place of the desired bit in the byte */
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
