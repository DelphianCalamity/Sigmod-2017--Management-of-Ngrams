#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bursts.h"
#include "errorMessages.h"
#include "ngram.h"
#include "trieStructs.h"

// adds a command at the end of the currently last burst
void addCommand(char com, NgramVector *ngram){
	Burst *temp;
	int i;

	if ((temp = malloc(sizeof(Burst))) == NULL){
		getError(1);
		exit(1);
	}
	// printf("\n");
	// for (i=0; i<ngram->words; i++)
	// 	printf("%s ", ngram->ngram[i]);
	temp->command = com;
	temp->ngram = ngram;
	// printf("\nFrom Bursts: \n");
	// for (i=0; i<temp->ngram->words; i++)
	// 	printf("%s ", temp->ngram->ngram[i]);
	temp->next = NULL;
	if (burstListEnd->start == NULL)			// first command in this burst
		burstListEnd->start = temp;
	else
		burstListEnd->end->next = temp;			// add command at the end of the burst
	burstListEnd->end = temp;
	// printf("\nFrom List:\n");
	// for (i=0; i<burstListEnd->end->ngram->words; i++)
	// 	printf("%s ", burstListEnd->end->ngram->ngram[i]);
}

// adds a burst at the end of BurstList
void addBurst(void){
	BurstList *temp;

	if ((temp = malloc(sizeof(BurstList))) == NULL){
		getError(1);
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
	int i;

	while (burst->start != NULL){
		temp=burst->start;
//printf(" number of words = %d\n", temp->ngram->words);
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
