#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bursts.h"
#include "parsing.h"

void readInputFile(char *inputFile){
	int i, max, words;
	char buffer[1024], stop=0, **ngrams, *token;
	FILE *fp;


	if ((fp = fopen(inputFile, "r")) == NULL){				// open graph file
		//perror(printError(2));
		exit(2);
	}

	while (!stop){											// while there are still uninserted n-grams
		if (fgets(buffer, 1024, fp) == NULL){
			fclose(fp);
			//perror(printError(3));
			exit(3);
		}
		if (buffer[0] == 'S'){
			stop = 1;
		}
		else{
			i = strlen(buffer);
			if (buffer[i-1] == '\n'){							// remove \n for easier handling
				buffer[i-1] = '\0';
			}
			i = 0;
			
			if ((ngrams = malloc(MAX*sizeof(char *))) == NULL){				// the new vector
				//perror(printError(1));
				exit(1);
			}
			max = MAX;														// max number of words the vector can currently hold
			ngrams[0] = strtok(buffer, " ");								// first word
			words = 1;														// current number of words in the vector
			while ((token = strtok(NULL, " ")) != NULL){					// while there are still some words
				if (words == 10){
					if ((ngrams = realloc(ngrams, 2*max)) == NULL){			// not enough space in vector, double it
						//perror(printError(1));
						exit(1);
					}
					max *= 2;
				}
				ngrams[words] = token;
				words++;
			}

			//Insert the n-gram
		}
	}

	fclose(fp);

}



void readQueryFile(char *queryFile){
	int i, max, words;
	char buffer[1024], stop=0, **ngrams, *token, command, burstFlag=1;
	FILE *fp;


	if ((fp = fopen(queryFile, "r")) == NULL){				// open graph file
		//perror(printError(2));
		exit(2);
	}

	while (fgets(buffer, 1024, fp) != NULL){
		if (burstFlag){
			addBurst();
			burstFlag = 0;
		}

		if (buffer[0] == 'F'){
			burstFlag = 1;
		}
		else {
			i = strlen(buffer);
			if (buffer[i-1] == '\n'){							// remove \n for easier handling
				buffer[i-1] = '\0';
			}
			i = 0;
			
			command = buffer[0];

			if ((ngrams = malloc(MAX*sizeof(char *))) == NULL){				// the new vector
				//perror(printError(1));
				exit(1);
			}
			max = MAX;														// max number of words the vector can currently hold
			ngrams[0] = strtok(&buffer[2], " ");							// first word
			words = 1;														// current number of words in the vector
			while ((token = strtok(NULL, " ")) != NULL){					// while there are still some words
				if (words == 10){
					if ((ngrams = realloc(ngrams, 2*max)) == NULL){			// not enough space in vector, double it
						//perror(printError(1));
						exit(1);
					}
					max *= 2;
				}
				ngrams[words] = token;
				words++;
			}

			addCommand(command, ngram, words);
		}
	}

	fclose(fp);
}