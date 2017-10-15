all: ngrams

ngrams: bursts.o errorMessages.o main.o ngrams.o parsing.o trieStructs.o
	gcc bursts.o errorMessages.o main.o ngrams.o parsing.o trieStructs.o  -o ngrams

bursts.o: bursts.c
	gcc -c bursts.c

errorMessages.o: errorMessages.c
	gcc -c errorMessages.c

main.o: main.c
	gcc -c main.c

ngrams.o: ngrams.c
    gcc -c ngrams.c

parsing.o: parsing.c
	gcc -c parsing.c

trieStructs.o: trieStructs.c
	gcc -c trieStructs.c

clean: 
	rm *o
