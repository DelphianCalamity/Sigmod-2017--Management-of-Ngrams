CC = gcc
MAINSRC = main.c
SRCS = bursts.c errorMessages.c ngram.c parsing.c stack.c trieStructs.c Hashtable.c compactTree.c bloomFilter.c topK_Hashtable.c murmur.c JobScheduler.c
HDRS = bursts.h errorMessages.h ngram.h parsing.h stack.h trieStructs.h ./Hashtable/Hashtable.h ./CompactTrie/compactTree.h ./BloomFilter/bloomFilter.h ./TopK/topK_Hashtable.h ./BloomFilter/murmur.h ./Threads/JobScheduler.h
PROG = ngrams
FLAGS = -Wall -g -O3
OBJS = $(SRCS:.c=.o)

PROG1 = unitTest
SRCS1 = unit_testing.c unitTest.c
HDRS1 = ./UnitTesting/unit_testing.h
OBJS1 = $(SRCS1:.c=.o)

#all: $(PROG) $(PROG1)

$(PROG) :  $(OBJS)
	$(CC) $(OBJS) main.o -o $(PROG) -lm -lpthread

$(PROG1) :  $(OBJS1) $(OBJS)
	$(CC) $(OBJS1) $(OBJS) -o $(PROG1) -lm -lpthread


main.o : main.c $(HDRS)
	$(CC) main.c -c $(FLAGS)
errorMessages.o : errorMessages.c $(HDRS)
	$(CC) errorMessages.c -c $(FLAGS)
bursts.o : bursts.c $(HDRS)
	$(CC) bursts.c -c $(FLAGS)
ngram.o : ngram.c $(HDRS)
	$(CC) ngram.c -c $(FLAGS)
parsing.o : parsing.c $(HDRS)
	$(CC) parsing.c -c $(FLAGS)
trieStructs.o : trieStructs.c $(HDRS)
	$(CC) trieStructs.c -c $(FLAGS)
stack.o : stack.c $(HDRS)
	$(CC) stack.c -c $(FLAGS)
Hashtable.o : ./Hashtable/Hashtable.c $(HDRS)
	$(CC) ./Hashtable/Hashtable.c -c $(FLAGS)
compactTree.o : ./CompactTrie/compactTree.c $(HDRS)
	$(CC) ./CompactTrie/compactTree.c -c $(FLAGS)
topK_Hashtable.o : ./TopK/topK_Hashtable.c $(HDRS)
	$(CC) ./TopK/topK_Hashtable.c -c $(FLAGS)
bloomFilter.o : ./BloomFilter/bloomFilter.c $(HDRS)
	$(CC) ./BloomFilter/bloomFilter.c -c $(FLAGS)
murmur.o : ./BloomFilter/murmur.c $(HDRS)
	$(CC) ./BloomFilter/murmur.c -c $(FLAGS)
JobScheduler.o: ./Threads/JobScheduler.c $(HDRS)
	$(CC) ./Threads/JobScheduler.c -c $(FLAGS)

unit_testing.o : ./UnitTesting/unit_testing.c $(HDRS1) $(HDRS)
	$(CC) ./UnitTesting/unit_testing.c -c $(FLAGS)
unitTest.o : ./UnitTesting/unitTest.c $(HDRS1) $(HDRS)
	$(CC) ./UnitTesting/unitTest.c -c $(FLAGS)

runss:	${PROG}
		./${PROG} -i small_static/small_static.init -q small_static/small_static.work
runsd:	${PROG}
		./${PROG} -i small_dynamic/small_dynamic.init -q small_dynamic/small_dynamic.work
runms:	${PROG}
		./${PROG} -i medium_static/medium_static.init -q medium_static/medium_static.work
runmd:	${PROG}
		./${PROG} -i medium_dynamic/medium_dynamic.init -q medium_dynamic/medium_dynamic.work
runls:	${PROG}
		./${PROG} -i large_static/large_static.init -q large_static/large_static.work
runld:	${PROG}
		./${PROG} -i large_dynamic/large_dynamic.init -q large_dynamic/large_dynamic.work

clean :
	rm -f core $(PROG) $(OBJS) $(PROG1) $(OBJS1)
TAGS : $(SRCS) $(HDRS)
	etags -t $(SRCS) $(HDRS)
