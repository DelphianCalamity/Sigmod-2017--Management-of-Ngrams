#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bursts.h"
#include "errorMessages.h"
#include "ngram.h"
#include "trieStructs.h"
#include "TopK/topK_Hashtable.h"

// adds a command at the end of the currently last burst
void addCommand(char com, NgramVector *ngram) {
	Burst *temp;

	temp = safemalloc(sizeof(Burst));
	temp->command = com;
	temp->ngram = ngram;
	temp->next = NULL;
	if (burstListEnd->start == NULL)	           // first command in this burst
		burstListEnd->start = temp;
	else
		burstListEnd->end->next = temp;            // add command at the end of the burst

	burstListEnd->end = temp;
}

// adds a burst at the end of BurstList
void addBurst(void) {

	BurstList *temp;
	temp = safemalloc(sizeof(BurstList));
	temp->start = NULL;
	temp->end = NULL;
	temp->next = NULL;
	if (burstListStart == NULL)					// first burst
		burstListStart = temp;
	else
		burstListEnd->next = temp;				// add burst to the end of the list
	burstListEnd = temp;
}

// executes all commands in current burst
void executeBurstCommands(BurstList *burst){

	Burst *temp;
	hashtable = topK_Hashtable_create(800, 5);

	while (burst->start != NULL){
		temp=burst->start;
		if (temp->command == 'A'){
			trieInsertSort(temp->ngram);
        }
		else if (temp->command == 'D'){
			trieDeleteNgram(temp->ngram);
        }
		else if (temp->command == 'Q'){
			(*SearchPtr)(temp->ngram);
		}
		burst->start = burst->start->next;
		deleteWords(temp->ngram);
        deleteNgram(temp->ngram);
		free(temp);
	}

	topK_print_TopK(hashtable, burst->k);		//print the TopK ngrams
	//topK_Hashtable_print(hashtable);
	topK_Hashtable_Destroy(hashtable);
}


// goes through all bursts
void processBursts(void){
	BurstList *temp;

	while (burstListStart != NULL){
		temp=burstListStart;
		executeBurstCommands(temp);
		burstListStart = burstListStart->next;
		free(temp);
	}
}
