#include <stdio.h>
#include <stdlib.h>
#include "List.h"


//A shoddy list of integers



int List_empty(Listptr ptr)         /* Checks if the list is empty*/
{   if(ptr->start == NULL)
        return 1;
    else return 0;
}

Listptr List_create(void)           /* Allocates space for the struct list and returns the pointer */
{   Listptr ptr;

    ptr = malloc(sizeof(List));
    ptr->start = NULL;
    ptr->end = NULL;
    ptr->size = 0;

    return ptr;
}


Nodeptr List_Insert_End(Listptr ptr, int num)
{
    Nodeptr nodeptr = malloc(sizeof(Node));         /*Creates the node*/
    nodeptr->n = num;
    nodeptr->next=NULL;
    nodeptr->prev=NULL;

    ptr->size++;
    
    if (List_empty(ptr))                    /*If Empty List*/
    {   
        ptr->start = nodeptr;
        ptr->end = nodeptr;
        nodeptr->next = NULL;
        nodeptr->prev = NULL;
    }
    else                                //Insert At End
    {
        Nodeptr nptr = ptr->end;
        nptr->next = nodeptr;
        nodeptr->prev = nptr;
        ptr->end = nodeptr;
    }
    
    return nodeptr;
}


Nodeptr List_Move_Start_At_End(Listptr ptr) {

	Nodeptr node = ptr->start;

	ptr->start = node->next;
	node->next->prev = NULL;

	node->next = NULL;
	node->prev = ptr->end;
	ptr->end->next = node;
	ptr->end = node;

	return node;
}

void List_Move_At_Start(Listptr ptr, Nodeptr node) {

	if(ptr->start == node)
		return;

	if (ptr->end == node) {				//From end
		node->prev->next = NULL;
		ptr->end = node->prev;
	}
	else {
		node->prev->next = node->next;
		node->next->prev = node->prev;
	}

		ptr->start->prev = node;
		node->next = ptr->start;
		node->prev = NULL;
		ptr->start = node;

}


void List_print(Listptr list) {
	Nodeptr ptr = list->start;

	while (ptr != NULL) {
		printf("%d,", ptr->n);
		ptr = ptr->next;
	}
	printf("\n");
}

void List_printBack(Listptr list) {
	Nodeptr ptr = list->end;

	while (ptr != NULL) {
		printf("%d,", ptr->n);
		ptr = ptr->prev;
	}
	printf("\n");
}

void List_destroy(Listptr list)
{
    Nodeptr node = list->start;
    Nodeptr ptr = list->start;

    while(ptr!=NULL)
    {   ptr = ptr->next;
        free(node);
        node = ptr;
    }

	list->start = NULL;
	list->end = NULL;
	list->size = 0;
//    free(list);
}