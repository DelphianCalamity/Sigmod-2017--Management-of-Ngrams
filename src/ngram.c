
#include <stdio.h>
#include <stdlib.h>
#include "ngram.h"

/*** Functions ***/

int Ngram_empty(Ngramptr ptr) {           /* Checks if the list is empty */
    if(ptr->start == NULL)
        return 1;
    else return 0;
}

Ngramptr Ngram_create(void) {             /* Allocates space for the list 's index and returns the pointer */
    
    Ngramptr ptr;

    ptr = malloc(sizeof(Ngram));
    ptr->start = NULL;
    ptr->end = NULL;
    ptr->size = 0;

    return ptr;
}

Wordptr Ngram_Insert_End(Ngramptr ptr, char* word) {
    
    if (word == NULL)
        return NULL;
    
    Wordptr nodeptr = malloc(sizeof(Word));                 /*Create the node*/
    strcpy(nodeptr->word, word);
    nodeptr->next=NULL;
    nodeptr->prev=NULL;

    ptr->size++;
    
    if (Ngram_empty(ptr)) {                                 /*If Empty List*/
        
        ptr->start = nodeptr;
        ptr->end = nodeptr;
        nodeptr->next = NULL;
        nodeptr->prev = NULL;
        
    }
    
    else {                                                   /*Insert At End*/
        Wordptr nptr = ptr->end;
        nptr->next = nodeptr;
        nodeptr->prev = nptr;
        ptr->end = nodeptr;
    }
    
    return nodeptr;
}


void Ngram_print(Ngramptr list) {

    if(Ngram_empty(list))
        return;
    
    Ngramptr ptr = list->start;
    while (ptr != NULL) {
        printf("%s ", ptr->word)
        ptr = ptr->next;
    }
}

void Ngram_destroy(Ngramptr list){
    
    Ngramptr node = list->start;
    Ngramptr ptr = list->start;

    while(ptr!=NULL){
        ptr = ptr->next;
        free(node);
        node = ptr;
    }
    free(list);
}


