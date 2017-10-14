all: ngrams

ngrams: bursts.o errorMessages.o main.o ngram.o trieStructs.o
	gcc bursts.o errorMessages.o main.o ngram.o trieStructs.o -o ngrams

bursts.o: bursts.c
	gcc -c bursts.c

errorMessages.o: errorMessages.c
	gcc -c errorMessages.c

main.o: main.c
	gcc -c main.c

ngram.o: ngram.c
	gcc -c ngram.c

parsing.o: parsing.c
	gcc -c parsing.c

trieStructs.o: trieStructs.c
	gcc -c trieStructs.c

clean: 
	rm *o
