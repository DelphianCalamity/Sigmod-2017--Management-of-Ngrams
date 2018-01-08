
#ifndef LIST_INT_H
#define	LIST_INT_H

typedef struct list_node Node;
typedef Node* Nodeptr;

typedef struct list List;
typedef List* Listptr;

struct list_node
{   int n;
	Nodeptr next;
	Nodeptr prev;

};

struct list
{   Nodeptr start;
	Nodeptr end;
	int size;
};

Listptr List_create(void);
int List_empty(Listptr ptr);
void List_printBack(Listptr list);
void List_print(Listptr list);
Nodeptr List_Insert_End(Listptr ptr, int num);

Nodeptr List_Move_Start_At_End(Listptr ptr);
void List_Move_At_Start(Listptr ptr, Nodeptr node);

void List_destroy(Listptr list);

#endif
