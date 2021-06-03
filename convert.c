//
//  convert.c
//  convert
//
//  Created by Tan Enhao on 2021/6/2.
//

#include "convert.h"

/*int localToRemote(int localID) {
    convertTable[idx] = localID;
    idx++;
    return idx-1;
}

int remoteToLocal(int remoteID) {
    return convertTable[remoteID];
}*/
IDADDR table[SIZE];

int InsertIDADDR (int id1, SOCKADDR_IN* ADDR) {
    table[id2].id1 = id1;
    memcpy(table[id2].ADDR, *ADDR, sizeof(SOCKADDR_IN));
    id2 = (id2 + 1) / SIZE;
    return (id2 - 1 + SIZE) / SIZE;
    
}

IDADDR* GetADDRbyID (int id2) {
    return &table[id2];
}
