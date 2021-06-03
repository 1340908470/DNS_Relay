//
// Created by 陈威豪 on 2021/5/29.
//

#ifndef DNS_STRUCT_H
#define DNS_STRUCT_H

#endif //DNS_STRUCT_H

typedef enum bool{false, true} bool;

//
// Created by 陈威豪 on 2021/5/29.
//

#ifndef DNS_STRUCT_H
#define DNS_STRUCT_H

#endif //DNS_STRUCT_H

#define DEF_DNS_ADDRESS "114.114.114.114"       //外部DNS服务器地址
#define LOCAL_ADDRESS "10.3.9.44"               //本地DNS服务器地址
#define ANY_ADDRESS "0.0.0.0"               //本地DNS服务器地址
#define DNS_PORT 53                             //进行DNS服务的53号端口

int DEBUGLEVEL = 0;

typedef struct {
    uint16_t ID;                //由客户程序设置并由服务器返回结果。客户程序通过它来确定响应与查询是否匹配

    uint16_t QR: 1;             //0表示查询报，1表示响应报
    uint16_t OPCODE: 4;         //通常值为0（标准查询），其他值为1（反向查询）和2（服务器状态请求）

    uint16_t RCODE: 4;          //响应码(Response coded)，仅用于响应报; 0:无差错，3表示域名差错
    uint16_t AA: 1;             //权威答案(Authoritative answer)
    uint16_t TC: 1;             //应答的总长度超512字节时，只返回前512个字节

    uint16_t Z: 3;              //
    uint16_t RA: 1;             //
    uint16_t RD: 1;             //

    uint16_t QDCOUNT;           //
    uint16_t ANCOUNT;
    uint16_t NSCOUNT;
    uint16_t ARCOUNT;
} DNSHeader;

typedef struct {
    char QNAMME[512];
    uint16_t QTYPE;
    uint16_t QCLASS;
} DNSQueries;

typedef struct {
    char buff[512];
    DNSHeader dnsHeader;
    DNSQueries dnsQueries;
    int questionLength;
    int answerOffset;
    uint32_t ip;
    size_t size_n;
} DNS;

