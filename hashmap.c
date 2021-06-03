//
// Created by 22973 on 2021/6/2.
//
#ifndef hashmap_c
#define hashmap_c

#include "windows.h"
#include "string.h"
#define MAPLEN 50000//哈希表数组长
#include "stdlib.h"
#include "stdio.h"
typedef struct MAPS{
    char* URL;
    char* ip;//URL-ip对
    struct MAPS *next;//同一哈希值的下一URL-ip对指针
}Map;//储存URL-ip对的结构体Map
Map map[MAPLEN];//结构体数组

int getKeyByURL(char* url){
    int key=0,i=0;

    for(i=0;url[i]!='\0';i++){
        key=key+url[i]*i;
    }

    key=key%MAPLEN;//哈希值生成方法，将每一位乘以自己所在的位数后取数组长的模
    return key;
}//根据URL计算哈希值并将哈希值返回
char* getIPByURL(char* url){
    char* IP=NULL;//将IP指针置空
    Map* mapget=map+getKeyByURL(url);//通过getKeyByURL算法找到URL-ip对存放位置所在并取出该指针
    if(mapget->ip==NULL)
        return NULL;//如果这个位置为空，则该对并不在哈希表中，返回空指针
    do{
        if(!strcmp(mapget->URL,url)){

            IP=(char*) malloc(strlen(mapget->ip)*8);
            strcpy(IP,mapget->ip);
            break;
        }//如果这个位置不为空，则比较里面存储的URL和导入的URL，相等则将URL对应的ip传出
        mapget=mapget->next;
    }while(mapget!=NULL);
    return IP;
};//通过URL在哈希表中寻得对应的ip
int insertMap(char* URL,char* ip){

    Map* mapget=map+getKeyByURL(URL);//找到URL-ip对要插入的位置

    if(mapget->ip==0){
        mapget->ip=(char*) malloc(strlen(ip)*8);
        mapget->URL=(char*) malloc(strlen(URL)*8);
        strcpy(mapget->ip,ip);
        strcpy(mapget->URL,URL);
    }//如果这个地方为空，则直接分配空间、插入结构体
    else{
        while(mapget->next!=NULL)
            mapget=mapget->next;
        mapget->next=(Map*)malloc(5);
        mapget=mapget->next;
        mapget->ip=(char*) malloc(strlen(ip)*8);
        mapget->URL=(char*) malloc(strlen(URL)*8);
        strcpy(mapget->ip,ip);
        strcpy(mapget->URL,URL);
        mapget->next=NULL;
    }//如果这个地方不为空，则通过next迭代到这个位置的最下层并将新生成的结构体插入
    return getKeyByURL(URL);//返回插入位置
}//将输入的URL-ip对放入hashmap中并返回插入的位置
#endif

