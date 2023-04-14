#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <dlfcn.h>




char* encode(char* string){

    char* temp = (char*)malloc(strlen(string));
    if(temp==NULL){
        printf("error");
        return "";
    }
    strcpy(temp,string);

    for (int i = 0; i<strlen(temp); i++) {
        temp[i]=temp[i]-3;
    }
    return temp;
}
char* decode(char* string){

    char* temp = (char*)malloc(strlen(string));
    if(temp==NULL){
        printf("error");
        return "";
    }
    strcpy(temp,string);

    for (int i = 0; i<strlen(temp); i++) {
        temp[i]=temp[i]+3;
    }
    return temp;
}
