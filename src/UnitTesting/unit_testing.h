#ifndef UNIT_TESTING_H
#define UNIT_TESTING_H

#include "../ngram.h"

void testBinarySearch();
void testSearch();
NgramVector* testCreateNgram(char *, ssize_t);
void testInsert();
void confirmExistence(NgramVector*);
int checkSearch(NgramVector*);
void testCreatTestCase();
void testDelete();

#endif //UNIT_TESTING_H
