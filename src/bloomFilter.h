#ifndef __BLOOMFILTER_H
#define __BLOOMFILTER_H

#include <stdbool.h>
#include "ngram.h"

#define BLOOMSIZE 16384			// it's actually 8 times more, since we're playing with bits, not whole bytes
#define K 10

char *bloomfilter;
unsigned int *cells;

bool findInBloom(NgramVector*);

#endif