#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bursts.h"
#include "ngram.h"
#include "trieStructs.h"
#include "errorMessages.h"
#include "TopK/topK_Hashtable.h"


void burst_init() {
	burst.numOfJobs = 0;
	burst.capacity = CMD_INIT;
	burst.jobs = safemalloc(sizeof(Job) * CMD_INIT);
}

void addCommand(char com, NgramVector *ngram) {

	if (burst.numOfJobs == burst.capacity) {
		burst.capacity *= 2;
		burst.jobs = saferealloc(burst.jobs, burst.capacity * sizeof(Job));
	}
	burst.jobs[burst.numOfJobs].command = com;
	burst.jobs[burst.numOfJobs].ngram = ngram;
	burst.numOfJobs++;
}

void executeBurstCommands() {

	int i;
	hashtable = topK_Hashtable_create(800, 5);

	for (i=0; i<burst.numOfJobs; i++) {
		executeCommand(&burst.jobs[i]);
	}

	//JobScheduler_execute_all_jobs();					//Replaces the command on top
	//sleep(10);
	//JobScheduler_wait_all_tasks_finish();
	//JobScheduler_Reset();

	topK_print_TopK(hashtable, burst.k);				//print the TopK ngrams
	//topK_Hashtable_print(hashtable);
	topK_Hashtable_Destroy(hashtable);
}

void executeCommand(Job* job) {

	if (job->command == 'A'){
		trieInsertSort(job->ngram);
	}
	else if (job->command == 'D'){
		trieDeleteNgram(job->ngram);
	}
	else if (job->command == 'Q'){
		(*SearchPtr)(job->ngram);
	}

	deleteWords(job->ngram);
	deleteNgram(job->ngram);
}
