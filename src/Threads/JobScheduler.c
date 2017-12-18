#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "JobScheduler.h"
#include "../bursts.h"
#include "../errorMessages.h"
#include "../trieStructs.h"

void JobScheduler_Init() {

	int i;
	jobScheduler.end = 0;
	jobScheduler.start = 0;
	jobScheduler.qcapacity = QINIT;
	jobScheduler.thread_pool_size = THREADPOOL;

	jobScheduler.queue = safemalloc(jobScheduler.qcapacity * sizeof(Job*));                        			//Queue
	pthread_mutex_init (&jobScheduler.queue_mutex , NULL);
	pthread_cond_init(&jobScheduler.queue_empty, NULL);
	pthread_cond_init(&jobScheduler.wait, NULL);

	jobScheduler.workers = safemalloc(jobScheduler.thread_pool_size * sizeof(pthread_t));                      	//Thread Pool
	for(i=0; i < jobScheduler.thread_pool_size; i++){                                            				//Creation of the workers
		if(pthread_create(&jobScheduler.workers[i], NULL, worker, NULL)){
			perror("pthread_create");
			exit(1);
		}
	}
}

void JobScheduler_Reset() {

	jobScheduler.end = 0;
	jobScheduler.start = 0;
}

void JobScheduler_SubmitJob(Job *job) {

	//Locking the mutex is not necessary now
	pthread_mutex_lock(&jobScheduler.queue_mutex);           						//LOCK

	if(jobScheduler.end == jobScheduler.qcapacity) {								// If there is no space in queue, reallocate
		jobScheduler.qcapacity *= 2;
		jobScheduler.queue = saferealloc(jobScheduler.queue, jobScheduler.qcapacity * sizeof(Job*));
	}
		jobScheduler.queue[jobScheduler.end] = job;                   			// Scheduler places the job in the queue
		jobScheduler.end++;

	pthread_mutex_unlock(&jobScheduler.queue_mutex);         						 //UNLOCK
}


void JobScheduler_execute_all_jobs() {

	int i;
	pthread_mutex_lock(&jobScheduler.queue_mutex);           						//LOCK

	for(i=0; i<=jobScheduler.end; i++) {
		pthread_cond_signal(&jobScheduler.queue_empty);                             // Wake up waiting workers
	}

	pthread_mutex_unlock(&jobScheduler.queue_mutex);         						//UNLOCK
}


void JobScheduler_wait_all_tasks_finish() {

	pthread_mutex_lock(&jobScheduler.queue_mutex);           						//LOCK
	while(jobScheduler.start != jobScheduler.end) {
		pthread_cond_wait(&jobScheduler.wait, &jobScheduler.queue_mutex);
	}
	pthread_mutex_unlock(&jobScheduler.queue_mutex);         						//UNLOCK
}



void *worker(void *args) {
	Job *job;

	while (1) {
		pthread_mutex_lock(&jobScheduler.queue_mutex);               				//LOCK

		while (jobScheduler.start == jobScheduler.end) {                            //while no job
			pthread_cond_wait(&jobScheduler.queue_empty, &jobScheduler.queue_mutex);
		}

		job = jobScheduler.queue[jobScheduler.start];                           				//reads the job
		jobScheduler.start++;

		pthread_mutex_unlock(&jobScheduler.queue_mutex);             							//UNLOCK

		//Process burst
		executeCommand(job);

		//pthread_mutex_lock(&jobScheduler.queue_mutex);               				//LOCK
		pthread_cond_signal(&jobScheduler.wait);
		//pthread_mutex_unlock(&queue_mutex);             							//UNLOCK
	}
}