#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <dlfcn.h>




char* encode(char *temp){
    for (int i = 0; i<strlen(temp); i++) {
        temp[i] = temp[i]+3;
    }
    return temp;
}
char* decode(char *temp){

    for (int i = 0; i<strlen(temp); i++) {
        temp[i] = temp[i]-3;
    }
    return temp;
}
