CC = gcc
SRCS = bursts.c errorMessages.c main.c ngram.c parsing.c stack.c trieStructs.c Hashtable.c compactTree.c bloomFilter.c topK_Hashtable.c murmur.c
HDRS = bursts.h errorMessages.h ngram.h parsing.h stack.h trieStructs.h ./Hashtable/Hashtable.h compactTree.h ./BloomFilter/bloomFilter.h ./TopK/topK_Hashtable.h ./BloomFilter/murmur.h
PROG = ngrams
FLAGS = -Wall -g -O3
OBJS = $(SRCS:.c=.o)

PROG1 = unitTest
SRCS1 = unit_testing.c unitTest.c errorMessages.c ngram.c stack.c trieStructs.c
HDRS1 = ./UnitTesting/unit_testing.h errorMessages.h ngram.h stack.h trieStructs.h
OBJS1 = $(SRCS1:.c=.o)

#all: $(PROG) $(PROG1)

$(PROG) :  $(OBJS)
	$(CC) $(OBJS) -o $(PROG) -lm

$(PROG1) :  $(OBJS1)
	$(CC) $(OBJS1) -o $(PROG1)


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
compactTree.o : compactTree.c $(HDRS)
	$(CC) compactTree.c -c $(FLAGS)
topK_Hashtable.o : ./TopK/topK_Hashtable.c $(HDRS)
	$(CC) ./TopK/topK_Hashtable.c -c $(FLAGS)
bloomFilter.o : ./BloomFilter/bloomFilter.c $(HDRS)
	$(CC) ./BloomFilter/bloomFilter.c -c $(FLAGS)
murmur.o : ./BloomFilter/murmur.c $(HDRS)
	$(CC) ./BloomFilter/murmur.c -c $(FLAGS)


unit_testing.o : ./UnitTesting/unit_testing.c $(HDRS1)
	$(CC) ./UnitTesting/unit_testing.c -c $(FLAGS)
unitTest.o : ./UnitTesting/unitTest.c $(HDRS1)
	$(CC) ./UnitTesting/unitTest.c -c $(FLAGS)

clean :
	rm -f core $(PROG) $(OBJS) $(PROG1) $(OBJS1)
TAGS : $(SRCS) $(HDRS)
	etags -t $(SRCS) $(HDRS)
