#ifndef __BURSTS_H__
#define __BURSTS_H__

#include "ngram.h"
#include "trieStructs.h"
#include "Threads/JobScheduler.h"

#define CMD_INIT 20

typedef struct burst Burst;
typedef struct job Job;

struct burst {
	int k;
	int numOfJobs;
	int capacity;
	Job* jobs;
};

struct job {
	int id;
	int topkid;
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
void executeBurstCommands();
void addCommand(char, NgramVector*);

#endif

