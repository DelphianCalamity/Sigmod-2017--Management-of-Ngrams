#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bursts.h"
#include "ngram.h"
#include "trieStructs.h"

// adds a command at the end of the currently last burst
void addCommand(char com, NgramVector *ngram){
	Burst *temp;

	if ((temp = malloc(sizeof(Burst))) == NULL){
		//perror(printError(1));
		exit(1);
	}

	temp->command = com;
	temp->ngram = ngram;
	temp->next = NULL;
	if (burstListEnd->start == NULL)			// first command in this burst
		burstListEnd->start = temp;
	else
		burstListEnd->end->next = temp;			// add command at the end of the burst
	burstListEnd->end = temp;
}

// adds a burst at the end of BurstList
void addBurst(void){
	BurstList *temp;

	if ((temp = malloc(sizeof(BurstList))) == NULL){
		//perror(printError(1));
		exit(1);
	}

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

	while (burst->start != NULL){
		temp=burst->start;
		if (temp->command == 'A'){
			trieInsertSort(temp->ngram);
		}
		else if (temp->command == 'D'){
			trieDeleteNgram(temp->ngram);
			deleteWords(temp->ngram);
		}
		else{
			trieSearch(temp->ngram);
			deleteWords(temp->ngram);
		}
		burst->start = burst->start->next;
		deleteNgram(temp->ngram);
		free(temp);
	}
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
