#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bursts.h"
#include "ngram.h"
#include "trieStructs.h"
#include "errorMessages.h"
#include "TopK/topK_Hashtable.h"
#include "CompactTrie/compactTree.h"

void burst_init() {
	int i;
	for (i=0; i<2; i++) {
		burst[i].numOfJobs = 0;
		burst[i].capacity = CMD_INIT;
		burst[i].jobs = safemalloc(sizeof(Job) * CMD_INIT);
	}

	if (pthread_create(&burst_processor, NULL, processBurst, &j)){
		perror("pthread_create");
		exit(1);
	}
}

void addCommand(char com, NgramVector *ngram, int i) {

	if (burst[i].numOfJobs == burst[i].capacity) {
		burst[i].capacity *= 2;
		burst[i].jobs = saferealloc(burst[i].jobs, burst[i].capacity * sizeof(Job));
	}
	burst[i].jobs[burst[i].numOfJobs].command = com;
	burst[i].jobs[burst[i].numOfJobs].ngram = ngram;
	burst[i].numOfJobs++;
}


void executeCommand(Job* job) {

	trieSearch(job->ngram, job->Q_version, job->id);
	deleteWords(job->ngram);
	deleteNgram(job->ngram);
}

void executeStaticCommand(Job* job) {

	trieSearch_Static(job->ngram, job->id);
	deleteWords(job->ngram);
	deleteNgram(job->ngram);
}

void *processBurst() {

	int i=0, id=0;

	if (j < 0) {
		j = 0;
		pthread_exit(NULL);
	}

	hashtable = topK_Hashtable_create(800, 5);

	while (i < burst[j].numOfJobs) {

		switch(burst[j].jobs[i].command) {

			case 'D':   trieFakeDeleteNgram(burst[j].jobs[i].ngram);
				i++;
				break;

			case 'A':   trieInsertSort(burst[j].jobs[i].ngram);
				deleteWords(burst[j].jobs[i].ngram);
				deleteNgram(burst[j].jobs[i].ngram);
				i++;
				break;

			case 'Q':	while (i < burst[j].numOfJobs && burst[j].jobs[i].command == 'Q') {

					burst[j].jobs[i].id = id++;
					burst[j].jobs[i].Q_version = trieRoot->current_version;
					JobScheduler_SubmitJob(&burst[j].jobs[i]);
					i++;
				}

				trieRoot->current_version++;
				break;
		}
	}

	if (queryBuffer.capacity < id) {
		queryBuffer.capacity = id;
		queryBuffer.sizes = saferealloc(queryBuffer.sizes, sizeof(int) * id);
		queryBuffer.buffer = saferealloc(queryBuffer.buffer, sizeof(char*) * id);
		queryBuffer.capacities = saferealloc(queryBuffer.capacities, sizeof(int) * id);
	}

	JobScheduler_execute_all_jobs();
	JobScheduler_wait_all_tasks_finish();


	// End of Burst - Delete Ngrams                 -- to be optimized
	for (i=0; i < burst[j].numOfJobs; i++) {

		if (burst[j].jobs[i].command == 'D') {

			trieDeleteNgram(burst[j].jobs[i].ngram);
			deleteWords(burst[j].jobs[i].ngram);
			deleteNgram(burst[j].jobs[i].ngram);
		}
	}

	// print burst ngrams
	for (i=0; i < id; i++) {
		printf("%s\n", queryBuffer.buffer[i]);
		free(queryBuffer.buffer[i]);
	}
	//topK

	topK_print_TopK(hashtable, burst[j].k);				//print the TopK ngrams
	//topK_Hashtable_print(hashtable);
	topK_Hashtable_Destroy(hashtable);

	burst[j].numOfJobs = 0;

	j = (j+1)%2;

	pthread_exit(NULL);

}


void *processBurstStatic() {

	int i=0, id=0;

	if (j < 0) {
		j = 0;
		pthread_exit(NULL);
	}

	hashtable = topK_Hashtable_create(800, 5);

	while (i < burst[j].numOfJobs) {

		burst[j].jobs[i].id = id++;
		JobScheduler_SubmitJob(&burst[j].jobs[i]);
		i++;
	}

	if (queryBuffer.capacity < id) {
		queryBuffer.capacity = id;
		queryBuffer.sizes = saferealloc(queryBuffer.sizes, sizeof(int) * id);
		queryBuffer.buffer = saferealloc(queryBuffer.buffer, sizeof(char*) * id);
		queryBuffer.capacities = saferealloc(queryBuffer.capacities, sizeof(int) * id);
	}

	JobScheduler_execute_all_jobs();
	JobScheduler_wait_all_tasks_finish();

	// print burst ngrams
	for (i=0; i < id; i++) {
		printf("%s\n", queryBuffer.buffer[i]);
		free(queryBuffer.buffer[i]);
	}

	//topK
	topK_print_TopK(hashtable, burst[j].k);				//print the TopK ngrams
	//topK_Hashtable_print(hashtable);
	topK_Hashtable_Destroy(hashtable);

	burst[j].numOfJobs = 0;

	j = (j+1)%2;

	pthread_exit(NULL);
}