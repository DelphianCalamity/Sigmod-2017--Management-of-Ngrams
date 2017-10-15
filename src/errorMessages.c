//
// Created by Stathis on 10/12/2017.
//

char * getError(int code){
    switch (code) {
        case 1:  return("Error in memory allocation") ;

        case 2:  return("Error in memory reallocation");

        case 3:  return("Error in file opening");

        case 4:  return("") ;

        case 5:  return("") ;

    }
}