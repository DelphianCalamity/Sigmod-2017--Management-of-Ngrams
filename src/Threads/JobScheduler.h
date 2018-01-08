#ifndef JOBSCHEDULER_H
#define JOBSCHEDULER_H

#include "../bursts.h"
#include <pthread.h>

#define QINIT 15;
#define THREADPOOL 12;

typedef struct job Job;
typedef struct jobScheduler JobScheduler;

struct jobScheduler {

	int end;
	int start;
	int current;
	int counter;
	int qcapacity;
	int thread_pool_size;
	char kill;

	Job** queue;					// Jobs queue
	pthread_t* workers;				// Thread Pool

	//Mutex
	pthread_mutex_t queue_mutex;

	//Condition Variables
	pthread_cond_t queue_empty;
	pthread_cond_t wait;
};

void JobScheduler_Init();
void JobScheduler_SubmitJob(Job*);
void JobScheduler_execute_all_jobs();
void JobScheduler_wait_all_tasks_finish();
void JobScheduler_Destroy();
void *worker(void*);

#endif