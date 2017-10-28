

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
