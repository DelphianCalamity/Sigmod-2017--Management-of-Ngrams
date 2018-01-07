#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "JobScheduler.h"
#include "../errorMessages.h"


void *worker() {
	Job *job;

	while (1) {

		pthread_mutex_lock(&jobScheduler.queue_mutex);

		while (jobScheduler.kill == 0 && jobScheduler.end == jobScheduler.start) {              // while no job in queue
			pthread_cond_wait(&jobScheduler.queue_empty, &jobScheduler.queue_mutex);
		}

		if (jobScheduler.kill == 1) {
			pthread_mutex_unlock(&jobScheduler.queue_mutex);
			pthread_exit(NULL);
		}

		job = jobScheduler.queue[jobScheduler.start];                           				// reads the job
		jobScheduler.start++;
		pthread_mutex_unlock(&jobScheduler.queue_mutex);

		(*commandsPtr)(job);

		pthread_mutex_lock(&jobScheduler.queue_mutex);
		jobScheduler.counter++;
		pthread_cond_signal(&jobScheduler.wait);
		pthread_mutex_unlock(&jobScheduler.queue_mutex);
	}
}

void JobScheduler_Init() {

	int i;

	jobScheduler.end = 0;
	jobScheduler.start = 0;
	jobScheduler.current = 0;
	jobScheduler.counter = 0;
	jobScheduler.qcapacity = QINIT;
	jobScheduler.thread_pool_size = THREADPOOL;
	jobScheduler.kill = 0;

	jobScheduler.queue = safemalloc(jobScheduler.qcapacity * sizeof(Job*));                        			    //Queue
	pthread_mutex_init(&jobScheduler.queue_mutex, NULL);
	pthread_cond_init(&jobScheduler.queue_empty, NULL);
	pthread_cond_init(&jobScheduler.wait, NULL);


	jobScheduler.workers = safemalloc(jobScheduler.thread_pool_size * sizeof(pthread_t));                      	//Thread Pool
	for (i=0; i < jobScheduler.thread_pool_size; i++){                                            				//Creation of the workers
		if (pthread_create(&jobScheduler.workers[i], NULL, worker, NULL)){
			perror("pthread_create");
			exit(1);
		}
	}
}



void JobScheduler_SubmitJob(Job *job) {

	if (jobScheduler.current == jobScheduler.qcapacity) {										// If there is no space in queue, reallocate
		jobScheduler.qcapacity *= 2;
		jobScheduler.queue = saferealloc(jobScheduler.queue, jobScheduler.qcapacity * sizeof(Job*));
	}
	jobScheduler.queue[jobScheduler.current] = job;                                     		// Scheduler places the job in the queue
	jobScheduler.current++;
}


void JobScheduler_execute_all_jobs() {

	int i;
	pthread_mutex_lock(&jobScheduler.queue_mutex);

	jobScheduler.end = jobScheduler.current;

	//could be replaced by broadcast
//	pthread_cond_broadcast(&jobScheduler.queue_empty);

	for (i=0; i<=jobScheduler.end; i++) {
		pthread_cond_signal(&jobScheduler.queue_empty);                             			// Wake up waiting workers
	}

	pthread_mutex_unlock(&jobScheduler.queue_mutex);
}


void JobScheduler_wait_all_tasks_finish() {

	pthread_mutex_lock(&jobScheduler.queue_mutex);

	while (jobScheduler.counter < jobScheduler.end) {
		pthread_cond_wait(&jobScheduler.wait, &jobScheduler.queue_mutex);
	}

	jobScheduler.end = 0;                                                                       // Reset
	jobScheduler.start = 0;
	jobScheduler.current = 0;
	jobScheduler.counter = 0;

	pthread_mutex_unlock(&jobScheduler.queue_mutex);
}





void JobScheduler_Destroy() {

	pthread_mutex_destroy(&jobScheduler.queue_mutex);
	pthread_cond_destroy(&jobScheduler.queue_empty);
	pthread_cond_destroy(&jobScheduler.wait);

	free(jobScheduler.queue);
	free(jobScheduler.workers);
}