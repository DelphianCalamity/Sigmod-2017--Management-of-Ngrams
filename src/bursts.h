#ifndef __BURSTS_H__
#define __BURSTS_H__

#include "ngram.h"

typedef struct burstList BurstList;
typedef struct burst Burst;



struct burstList{
	Burst *start;           // first command
	Burst *end;             // last command
	BurstList *next;        // next burst
};

struct burst{
	char command;           // Q, A or D, depends on the command
	NgramVector *ngram;
	Burst *next;            // next command
};

void addCommand(char, char **, int);
void addBurst(void);
void executeBurstCommands(BurstList *);
void processBursts(void);

#endif

BurstList *burstListStart;
BurstList *burstListEnd;

