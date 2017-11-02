#ifndef _ERRORMESSAGES_H
#define _ERRORMESSAGES_H

void getError(int);
void* safemalloc(int);
void* safecalloc(int num, int size);
void* saferealloc(void* ptr, int size);

#endif //_ERRORMESSAGES_H
