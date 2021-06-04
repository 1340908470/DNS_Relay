//
//  convert.h
//  convert
//
//  Created by Tan Enhao on 2021/6/2.
//

#ifndef convert_h
#define convert_h

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define SIZE 0xffff

int id2;

typedef struct {
    int id1;            // ID from local
    SOCKADDR_IN ADDR;        // local's address
} IDADDR;

// return id2, use "memcpy"
int InsertIDADDR (int id1, SOCKADDR_IN* ADDR);

// return addr
IDADDR* GetADDRbyID (int id2);

#endif /* convert_h */
