//
// Created by 22973 on 2021/6/2.
//
#define MAPLEN 1000
#ifndef DNS_HASHMAP_H
#define DNS_HASHMAP_H


typedef struct {
    char* URL;
    char* ip;
    struct Map *next;
}Map;
Map map[MAPLEN];
int getKeyByURL(char* url);
char* getIPByURL(char* url);
int insertMap(char* URL,char* ip);


#endif //DNS_HASHMAP_H
