#ifndef __BURSTS_H__
#define __BURSTS_H__

#include "ngram.h"
#include "trieStructs.h"

#define CMD_INIT 20

typedef struct burst Burst;
typedef struct job Job;
typedef struct qjob Qjob;

int j;
pthread_t burst_processor;

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

Burst burst[2];

void burst_init();
void *processBurst();
void executeCommand(Job*);
void addCommand(char, NgramVector*, int);

#endif

