//
// Created by 22973 on 2021/6/2.
//
#define MAPLEN 1000

#include "hashmap.h"
#include "String.h"

int getKeyByURL(char* url){
    int key,i;
    for(i=0;url[i]!='\0';i++)
        key=key/2+url[i];
    return key;
}
char* getIPByURL(char* url){
    char* IP=NULL;
    Map mapget=map[getKeyByURL(url)];
    if(mapget.ip==NULL)
        return NULL;
    do{
        if(!strcmp(mapget.URL,url)){
            IP=mapget.ip;
            break;
        }
        mapget=mapget.next;
    }while(mapget.next!=NULL)
    return IP;
};
int insertMap(char*URL,char* ip){
    printf("%s %s\n", URL,ip);
    Map mapget=map[getKeyByURL(URL)];
    if(mapget.ip==0){
        mapget.ip=ip;
        mapget.URL=URL;
    }
    else{
        while(mapget.next!=NULL)
            mapget=mapget.next;
        mapget.next=(Map*)malloc(sizeof(map));
        mapget.next.ip=ip;
        mapget.next.URL=URL;
        mapget.next=NULL;
    }
}