//
// Created by 22973 on 2021/6/2.
//
#ifndef hashmap_c
#define hashmap_c

#include "string.h"
#define MAPLEN 1000
#include "stdlib.h"
#include "stdio.h"
typedef struct MAPS{
    char* URL;
    char* ip;
    struct MAPS *next;
}Map;
Map map[MAPLEN];

int getKeyByURL(char* url){
    int key=0,i=0;
    for(i=0;url[i]!='\0';i++)
        key=key/2+url[i];
    return key;
}
char* getIPByURL(char* url){
    char* IP=NULL;
    Map* mapget=map+getKeyByURL(url);
    if(mapget->ip==NULL)
        return NULL;
    do{
        if(!strcmp(mapget->URL,url)){
            IP=mapget->ip;

            break;
        }
        mapget=mapget->next;
    }while(mapget->next!=NULL);
    return IP;
};
int insertMap(char* URL,char* ip){
    printf("%s %s\n", URL, ip);
    Map* mapget=map+getKeyByURL(URL);

    if(mapget->ip==0){
        mapget->ip=ip;
        mapget->URL=URL;
    }
    else{
        while(mapget->next!=NULL)
            mapget=mapget->next;
        mapget->next=(Map*)malloc(sizeof(Map));
        //Map newmap;
        mapget=mapget->next;
        mapget->ip=ip;
        mapget->URL=URL;
        mapget->next=NULL;
    }
    return getKeyByURL(URL);
}
#endif

