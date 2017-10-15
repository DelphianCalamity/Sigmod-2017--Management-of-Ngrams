CC = gcc
SRCS = bursts.c errorMessages.c main.c ngram.c parsing.c trieStructs.c
HDRS = bursts.h errorMessages.h ngram.h parsing.h trieStructs.h
PROG = ngrams
FLAGS = -Wall -g
OBJS = $(SRCS:.c=.o)

$(PROG) :  $(OBJS)
	$(CC) $(OBJS) -o $(PROG)

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
clean :
	rm -f core $(PROG) $(OBJS)
TAGS : $(SRCS) $(HDRS)
	etags -t $(SRCS) $(HDRS)
