typedef struct job Job;

#ifndef __BURSTS_H__
#define __BURSTS_H__

#include "ngram.h"
#include "trieStructs.h"

#define CMD_INIT 20


typedef struct burst Burst;


struct burst {
	int k;
	int numOfJobs;
	int capacity;
	Job* jobs;
};

struct job {
	int id;
	char command;           // Q, A or D, depends on the command
	int Q_version;
	NgramVector *ngram;
};

Burst burst;

void burst_init();
void processBurst();
void processBurstStatic();
void executeCommand(Job*);
void executeStaticCommand(Job*);
void addCommand(char, NgramVector*);

#endif

