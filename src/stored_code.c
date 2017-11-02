

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




//     Burst *temp = burstListStart->start;
//     //getchar();
//     while(temp != NULL){
//         //printf("%s     %c ",temp->next->ngram->ngram[1], temp->command);
//         getchar();
//         for (i=0; i<temp->ngram->words; i++){
//                         getchar();
//             printf("%s ", temp->ngram->ngram[i]);
//
//         }
//         printf("\n");
//         temp = temp->next;
//     }


//SEARCH WITH BUFFER
/****************************************************************************/
//
//char* trieSearch(NgramVector *ngramVector) {
//
//    int i, check=0;
//    TrieNode* root = trieRoot->root;
//    char* buffer = malloc(/*BUFFER_SIZE*/1000*sizeof(char));
//    int capacity = 1000;//BUFFER_SIZE;
//    buffer[0] = '\0';
//
//    for(i=0; i<ngramVector->words; i++){                                                    //For all root's children
//        trieSearch_Ngram(root, i, i, ngramVector, &buffer, &capacity, &check);
//        memset(buffer, 0, capacity*sizeof(char));
//    }
//    if (check > 0) {
//        trieRoot->lastQuery++;
//        printf("\b \n");
//    }
//    else printf("-1\n");
//
//
//    free(buffer);
//    return NULL;
//}
//
//void trieSearch_Ngram(TrieNode* node, int round, int i, NgramVector *ngramVector, char** buffer, int* capacity, int* check) {
//
//    int len, space;
//    int init_len = len = (int) strlen(*buffer);
//
//    BinaryResult br;
//
//    for (; i < ngramVector->words; i++) {
//        if (node == NULL)                                                                   //No more nodes
//            return;
//
//        trieBinarySearch(&br, node, ngramVector->ngram[i]);
//
//        if (br.found == 0 || node->children[br.position].deleted == 1)                      //If word not found or deleted
//            return;
//
//        //An ngram is found
//        if (node->children[br.position].is_final == 1 && node->children[br.position].visited < trieRoot->lastQuery) {
//
//            (*check)++;
//            node->children[br.position].visited = trieRoot->lastQuery;
//
//            for (; round <= i; round++) {
//                if((space=(int)strlen(ngramVector->ngram[round])) >= *capacity - len-1) {
//                    if ((*buffer = realloc(*buffer, 2 * (*capacity)*sizeof(char))) == NULL) {              //Re-allocate space
//                        getError(2);
//                        exit(1);
//                    }
//                    *capacity *= 2;
//                }
//                memcpy(*buffer + len, ngramVector->ngram[round], space*sizeof(char));
//                len += space+1;
//                (*buffer)[len-1]=' ';
//            }
//            (*buffer)[len]='\0';
//            printf("%s\b|", *buffer+init_len);
//        }
//
//        node = &node->children[br.position];
//    }
//}



//void trieDeleteNgram(NgramVector *ngram) {
//    int i;
//    Stack s;
//    BinaryResult br;
//    TrieNode *node = trieRoot->root;
//
//    initStack(&s);
//    for (i = 0; i < ngram->words; i++) {
//
//        trieBinarySearch(&br, node, ngram->ngram[i]);
//        if (!br.found || node->children[br.position].deleted){
//            deleteStack(&s);
//            return;
//        }
//        push(&s, node);
//        node = &(node->children[br.position]);
//    }
//
//    if (!node->is_final){
//        deleteStack(&s);
//        return;
//    }
//
//    node->is_final = 0;
//
//    while (notEmpty(&s)) {
//
//        if((i = node->maxChildren-node->emptySpace-1) >= 0) {                   // maxChildren != emptySpace
//            while (node->children[i].deleted) {                                     // free all deleted children after the last active one
//                free(node->children[i].word);
//                node->children[i].deleted = 0;
//                node->deletedChildren--;
//                node->emptySpace++;
//                i--;
//            }
//        }
//
//        if (node->is_final || node->emptySpace + node->deletedChildren < node->maxChildren) {   // there still are active children or is final
//            deleteStack(&s);
//            return;
//        }
//
//        // no active children, delete 'em all
//        free(node->children);                                                       // delete the array
//        node->deleted = 1;
//
//        node = pop(&s);
//        node->deletedChildren++;
//    }
//
//    //for root too
//    if((i = node->maxChildren-node->emptySpace-1) >= 0) {                       // maxChildren != emptySpace
//        while (node->children[i].deleted) {                                     // free all deleted children after the last active one
//			free(node->children[i].word);
//            node->children[i].deleted = 0;
//			node->deletedChildren--;
//			node->emptySpace++;
//			i--;
//    	}
//    }
//    //if root has no active children they must not be freed
//
//    deleteStack(&s);
//}