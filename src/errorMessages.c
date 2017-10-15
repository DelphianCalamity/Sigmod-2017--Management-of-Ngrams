//
// Created by Stathis on 10/12/2017.
//
#include <stdio.h>

void getError(int code){
    switch (code) {
        case 1:  perror("Error in memory allocation") ; break;

        case 2:  perror("Error in memory reallocation");break;

        case 3:  perror("Error in file opening");break;

    }
    return;
}
