#include <stdio.h>
#include <stdlib.h>
#include "bursts.h"
#include "errorMessages.h"
#include "CompactTrie/compactTree.h"
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

void executeCommand(Job *job) {

	trieSearch(job->ngram, job->Q_version, job->id);
	deleteWords(job->ngram);
	deleteNgram(job->ngram);
}

void executeStaticCommand(Job *job) {

	trieSearch_Static(job->ngram, job->id);
	deleteWords(job->ngram);
	deleteNgram(job->ngram);
}


void processBurst() {

	int i = 0, id = 0;

	hashtable = topK_Hashtable_create(800, 5);

	while (i < burst.numOfJobs) {

		switch (burst.jobs[i].command) {

			case 'D':
				trieFakeDeleteNgram(burst.jobs[i].ngram);
				i++;
				break;

			case 'A':
				trieInsertSort(burst.jobs[i].ngram);
				deleteWords(burst.jobs[i].ngram);
				deleteNgram(burst.jobs[i].ngram);
				i++;
				break;

			case 'Q':
				while (i < burst.numOfJobs && burst.jobs[i].command == 'Q') {
					burst.jobs[i].id = id++;
					burst.jobs[i].Q_version = trieRoot->current_version;
					JobScheduler_SubmitJob(&burst.jobs[i]);
					i++;
				}
				trieRoot->current_version++;
				break;

			default:
				break;
		}
	}

	if (queryBuffer.capacity < id) {
		queryBuffer.capacity = id;
		queryBuffer.sizes = saferealloc(queryBuffer.sizes, sizeof(int) * id);
		queryBuffer.buffer = saferealloc(queryBuffer.buffer, sizeof(char *) * id);
		queryBuffer.capacities = saferealloc(queryBuffer.capacities, sizeof(int) * id);
	}

	JobScheduler_execute_all_jobs();
	JobScheduler_wait_all_tasks_finish();


	/*End of Burst - Delete Ngrams*/
	for (i = 0; i < burst.numOfJobs; i++) {

		if (burst.jobs[i].command == 'D') {

			trieDeleteNgram(burst.jobs[i].ngram);
			deleteWords(burst.jobs[i].ngram);
			deleteNgram(burst.jobs[i].ngram);
		}
	}

	/*print burst ngrams*/
	for (i = 0; i < id; i++) {
		printf("%s\n", queryBuffer.buffer[i]);
		free(queryBuffer.buffer[i]);
	}

	//topK
	topK_print_TopK(hashtable, burst.k);				//print the TopK ngrams
	//topK_Hashtable_print(hashtable);
	topK_Hashtable_Destroy(hashtable);

}


void processBurstStatic() {

	int i = 0, id = 0;

	hashtable = topK_Hashtable_create(800, 5);

	while (i < burst.numOfJobs) {

		burst.jobs[i].id = id++;
		JobScheduler_SubmitJob(&burst.jobs[i]);
		i++;
	}

	if (queryBuffer.capacity < id) {
		queryBuffer.capacity = id;
		queryBuffer.sizes = saferealloc(queryBuffer.sizes, sizeof(int) * id);
		queryBuffer.buffer = saferealloc(queryBuffer.buffer, sizeof(char *) * id);
		queryBuffer.capacities = saferealloc(queryBuffer.capacities, sizeof(int) * id);
	}

	JobScheduler_execute_all_jobs();
	JobScheduler_wait_all_tasks_finish();

	// print burst ngrams
	for (i = 0; i < id; i++) {
		printf("%s\n", queryBuffer.buffer[i]);
		free(queryBuffer.buffer[i]);
	}

	//topK
	topK_print_TopK(hashtable, burst.k);				//print the TopK ngrams
	//topK_Hashtable_print(hashtable);
	topK_Hashtable_Destroy(hashtable);


}