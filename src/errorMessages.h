#ifndef _ERRORMESSAGES_H
#define _ERRORMESSAGES_H

void getError(int);
void* safemalloc(size_t);
void* safecalloc(size_t num, size_t size);
void* saferealloc(void* ptr, size_t size);

#endif //_ERRORMESSAGES_H
