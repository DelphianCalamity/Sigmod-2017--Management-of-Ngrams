#include <stdio.h>
#include <stdlib.h>
#include "trieStructs.h"
#include <string.h>
#include "errorMessages.h"
#include "ngram.h"


/*Initialization of trie root*/
void trieRootInit() {
    if ((trieRoot = malloc(sizeof(TrieRoot))) == NULL) {
        getError(1);
        exit(1);
    }
    if ((trieRoot->root = malloc(sizeof(TrieNode))) == NULL) {
        getError(1);
        exit(1);
    }
    trieNodeInit(NULL, "", trieRoot->root);
}

/*Initialization of a new trienode*/
void trieNodeInit(TrieNode *parent, char *word, TrieNode *child) {

    /*Initialize basic info of node*/
    child->emptySpace = MINSIZE;
    child->maxChildren = MINSIZE;
    child->deletedChildren = 0;
    child->is_final = 0;
    child->parentNode = parent;
    child->deleted = 0;

    /*Initialize the word for the new node*/
    // if ((child->word = malloc(sizeof(strlen(word)))) == NULL) {
    //     getError(1);
    //     exit(1);
    // }
    // strcpy(child->word, word);
    child->word = word;

    /*Initiliaze new node's children*/
    if ((child->children = malloc(MINSIZE * sizeof(TrieNode))) == NULL) {
        getError(1);
        exit(1);
    }
    memset(child->children, 0, MINSIZE * sizeof(TrieNode));

}

// void trieAddToChildren(TrieNode *parent, char *word) {
//     TrieNode *newChildren;
//     BinaryResult result;
//
//     /*Run binary search*/
//     result = trieBinarySearch(parent, word);
//     if (result.found = 0) {
//         if (parent->children[result.position - 1].deleted) {
//             parent->children[result.position].deleted = 0;
//             parent->deletedChildren -= 1;
//         } else {
//             /*If the parent node has no more space for new children, allocate extra space*/
//             if (parent->emptySpace == 0) {
//                 if ((newChildren = realloc(parent->children, (parent->maxChildren * 2) * sizeof(TrieNode))) == NULL) {
//                     getError(2);
//                     exit(2);
//                 }
//                 parent->children = newChildren;
//                 parent->emptySpace += parent->maxChildren;
//                 parent->maxChildren *= 2;
//             } else {
//                 memcpy(&parent->children[result.position + 1], &parent->children[result.position],
//                        (parent->maxChildren - parent->emptySpace - result.position) * sizeof(TrieNode));
//             }
//         }
//         /*Store the new child and update children count*/
//
//     }
//     return;
// }


//If found is 1, element exists and 'position' holds its actual position
//Otherwise, 'position' holds the position inside the array where element should be.

void trieBinarySearch(BinaryResult *br, TrieNode *parent, char *word) {

    TrieNode *children = parent->children;

    br->found = 0;

    int size = parent->maxChildren - parent->emptySpace;            //Size of occupied array
    int fst = 0, lst = size - 1;
    int middle = (fst + lst) / 2;

    if (lst < 0) {                                                  //Empty array
        br->position = 0;
        return;
    }

    while (fst <= lst) {

        if (strcmp(children[middle].word, word) < 0)
            fst = middle + 1;

        else if (strcmp(children[middle].word, word) == 0) {
            br->position = middle;
            br->found = 1;
            return;
        } else lst = middle - 1;

        middle = (fst + lst) / 2;
    }

    br->position = fst;
}

/****************************************************************************/

void trieSearch(NgramVector *ngramVector) {

    int i;
    int check=0;

    TrieNode *root = trieRoot->root;

    printf("SEARCH: \n");
    for (i = 0; i < ngramVector->words; i++)                                       //For all root's children
        trieSearch_Ngram(root, i, i, ngramVector, &check);

    if(check > 0)
        printf("\b \n");
    else printf("-1\n");

}

void trieSearch_Ngram(TrieNode *node, int round, int i, NgramVector *ngramVector, int* check) {

    int j;
    BinaryResult br;

    for (; i < ngramVector->words; i++) {
        if (node == NULL)                                                           //No more nodes
            return;

        trieBinarySearch(&br, node, ngramVector->ngram[i]);

        if (br.found == 0 || node->children[br.position].deleted == 1)              //If word not found or deleted
            return;

        if (node->children[br.position].is_final == 1) {                            //An ngram is found

            (*check)++;
            for(j=round; j<i; j++){
                printf("%s ", ngramVector->ngram[j]);
            }
            printf("%s|", ngramVector->ngram[j]);
        }

        node = &node->children[br.position];
    }
}

/****************************************************************************/

int trieInsertSort(NgramVector *ngramVector) {
    int i;
    TrieNode *parent;
    TrieNode *newChildren;
    BinaryResult result;

    char *word;
    //printf("mpike\n");
    parent = trieRoot->root;

    for (i = 0; i < ngramVector->words; i++) {
        word = ngramVector->ngram[i];

        //printf("tsekaro tin leksi %s\n", word);
        /*Run binary search*/
        trieBinarySearch(&result, parent, word);

        if (result.found == 0 || (result.found == 1 && parent->children[result.position].deleted == 1)) {
            /*If it's within the borders of the chidlren array*/
            if (result.position < parent->maxChildren) {
                /*If there is already available space*/
                if (parent->children[result.position].deleted == 1) {
                    // for (i=0; i<parent->maxChildren-parent->emptySpace; i++){
                    //     printf ("word is %s\n", parent->children[i].deleted);
                    // }
                    //printf("edo den prepei na mpeis pote %d, me position %d kai paidia %d\n",
                           //parent->children[result.position].deleted, result.position, parent->maxChildren);
                    getchar();
                    //parent->children[result.position].deleted = 0;
                    parent->deletedChildren -= 1;
                    free(parent->children[result.position].word);
                } else {
                    /*In between the other children*/
                    if (result.position < parent->maxChildren - parent->emptySpace) {
                        /*Creating space from deleted nodes*/
                        if (parent->emptySpace == 0 && parent->deletedChildren > 0) {
                            i = 0;
                            while (i < (parent->maxChildren - parent->emptySpace)) {
                                if (parent->children[i].deleted == 1) {
                                    int end = i + 1;
                                    while (parent->children[end].deleted == 1 &&
                                           end < parent->maxChildren - parent->emptySpace) {
                                        end++;
                                    }
                                    memmove(&parent->children[i], &parent->children[end],
                                            (parent->maxChildren - parent->emptySpace - end) * sizeof(TrieNode));
                                    parent->emptySpace += end - i;
                                    parent->deletedChildren -= end - i;
                                }
                                i++;
                            }
                            memset(&parent->children[parent->maxChildren - parent->emptySpace], 0,
                                   (parent->maxChildren - parent->emptySpace) * sizeof(TrieNode));
                        }

                        if (parent->emptySpace == 0) {
                            //printf("mpikame na megalosoumeeee gia to %s\n", parent->word);
                            if ((newChildren = realloc(parent->children,
                                                       (parent->maxChildren * 2) * sizeof(TrieNode))) == NULL) {
                                getError(2);
                                exit(2);
                            }
                            parent->children = newChildren;
                            parent->emptySpace += parent->maxChildren;
                            parent->maxChildren *= 2;
                        }
                        memmove(&parent->children[result.position + 1], &parent->children[result.position],
                                (parent->maxChildren - parent->emptySpace - result.position) * sizeof(TrieNode));
                        //free( parent->children[result.position].word);
                    }
                    // /*At the end of the children, there is enough space*/
                    // else{
                    //
                    // }
                }
            }
                /*If ngrams needs to go at the end of the children array, but there is no space*/
            else {
                //printf("mpikame na megalosoumeeee gia to %s\n", parent->word);
                if ((newChildren = realloc(parent->children, (parent->maxChildren * 2) * sizeof(TrieNode))) == NULL) {
                    getError(2);
                    exit(2);
                }
                parent->children = newChildren;
                parent->emptySpace += parent->maxChildren;
                parent->maxChildren *= 2;
            }

//            if ( i == ngramVector->words -1)
//                final = 1;
            /*Store the new child and update children count*/
            trieNodeInit(parent, word, &parent->children[result.position]);
            parent->emptySpace--;

        }
//        if ( i == ngramVector->words -1)
//            parent->children[result.position].is_final = 1;
        parent = &parent->children[result.position];

    }
    //if (parent->is_final == 0){
    parent->is_final = 1;
    //}


    return 0;
}

void trieDeleteNgram(NgramVector *ngram) {
    int i;
    BinaryResult br;
    TrieNode *node = trieRoot->root;
    for (i = 0; i < ngram->words; i++) {
        //printf("Searching %s...\n", ngram->ngram[i]);
        trieBinarySearch(&br, node, ngram->ngram[i]);
        if (!br.found || node->children[br.position].deleted)
            return;
        //printf("FOUND IT\n");
        node = &(node->children[br.position]);
    }
    //printf("final: %d\n", node->is_final);
    if (!node->is_final)
        return;

    node->is_final = 0;
    //printf("space left: %d | max: %d\n", node->emptySpace+node->deletedChildren, node->maxChildren);
    if (node->emptySpace + node->deletedChildren < node->maxChildren){           // there still are active children
        i = node->maxChildren-node->emptySpace-1;
    	while (node->children[i].deleted){				// free all deleted children after the last active one
			free(node->children[i].word);
			node->deletedChildren--;
			node->emptySpace++;
			i--;
    	}
        return;
    }

    // no active children, delete 'em all
    for (i = 0; i < node->deletedChildren; i++)                                 // delete the words of inactive children
        free(node->children[i].word);
    free(node->children);                                                   // delete the array
    node->deleted = 1;
    node = node->parentNode;


    //for the rest of the ngram
    while (node != trieRoot->root) {
        //printf("\nfinal: %d\n", node->is_final);
        if (node->is_final)                 // end of another ngram, return
            return;
        //printf("space left: %d | max: %d\n", node->emptySpace+node->deletedChildren, node->maxChildren);
        if (node->emptySpace + node->deletedChildren < node->maxChildren){           // there still are active children
        	i = node->maxChildren-node->emptySpace-1;
        	while (node->children[i].deleted){				// free all deleted children after the last active one
    			free(node->children[i].word);
    			node->deletedChildren--;
    			node->emptySpace++;
    			i--;
        	}
            return;
        }

        // no active children, delete 'em all
        for (i = 0; i < node->deletedChildren; i++)                                 // delete the words of inactive children
            free(node->children[i].word);
        free(node->children);                                                   // delete the array
        node->deleted = 1;
        node = node->parentNode;
    }
}

void trieFree() {

    TrieNode *node = trieRoot->root;
    trieRecursiveFree(node);

    free(trieRoot);
}

void trieRecursiveFree(TrieNode *node) {

    int i;
    for (i = 0; i < node->maxChildren - node->emptySpace; i++) {

        free(node->word);

        if (node->deleted == 1)
            continue;

        trieRecursiveFree(&node->children[i]);
    }

    free(node->children);
    free(node);
}
