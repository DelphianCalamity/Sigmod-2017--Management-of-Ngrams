#ifndef JOBSCHEDULER_H
#define JOBSCHEDULER_H

#include "../bursts.h"
#include <pthread.h>

#define QINIT 20;
#define THREADPOOL 20;

typedef struct job Job;
typedef struct jobScheduler JobScheduler;

struct jobScheduler {

	int end;
	int start;
	int qcapacity;
	int thread_pool_size;

	Job** queue;					// Jobs queue
	pthread_t* workers;				// Thread Pool

	//Mutexes
	pthread_mutex_t queue_mutex;
	//pthread_mutex_t* mutexes;

	//Condition Variable
	pthread_cond_t queue_empty;
	pthread_cond_t wait;
};


void JobScheduler_Init();
void JobScheduler_Reset();
void JobScheduler_SubmitJob();
void JobScheduler_execute_all_jobs();
void JobScheduler_wait_all_tasks_finish();
void *worker(void*);


#endif
