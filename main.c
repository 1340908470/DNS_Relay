#include "include.h"
#include "struct.h"
//#include "assert.h"
#include "hashmap.h"

// 获取DNS头部信息
void DNS_GetHead(DNS *dns) {
    uint16_t cur_word = htons(*(uint16_t *) ((*dns).buff + 2));
    (*dns).dnsHeader.ID = (int) cur_word;
    (*dns).dnsHeader.QR = (cur_word & 0x8000) >> 15;
    (*dns).dnsHeader.OPCODE = (cur_word & 0x7800) >> 11;
    (*dns).dnsHeader.AA = (cur_word & 0x0400) >> 10;
    (*dns).dnsHeader.TC = (cur_word & 0x0200) >> 9;
    (*dns).dnsHeader.RD = (cur_word & 0x0100) >> 8;
    (*dns).dnsHeader.RA = (cur_word & 0x0080) >> 7;
    (*dns).dnsHeader.Z = 0;
    (*dns).dnsHeader.RCODE = (cur_word & 0x000F);

    (*dns).dnsHeader.QDCOUNT = htons(*(uint16_t *) ((*dns).buff + 4));
    (*dns).dnsHeader.ANCOUNT = htons(*(uint16_t *) ((*dns).buff + 6));
    (*dns).dnsHeader.NSCOUNT = htons(*(uint16_t *) ((*dns).buff + 8));
    (*dns).dnsHeader.ARCOUNT = htons(*(uint16_t *) ((*dns).buff + 10));

    if (DEBUGLEVEL >= 3) {
        printf("DNS Header: (Hexadecimal)\n");
        printf("ID = %x  \t\t┇  QR = %x  \t\t\t┇  OPCODE = %x  \t\t┇  AA = %x\n",
               (*dns).dnsHeader.ID, (*dns).dnsHeader.QR, (*dns).dnsHeader.OPCODE, (*dns).dnsHeader.AA);
        printf("TC = %x  \t\t┇  RD = %x  \t\t\t┇  RA = %x  \t\t\t┇  RCODE = %x\n",
               (*dns).dnsHeader.TC, (*dns).dnsHeader.RD, (*dns).dnsHeader.RA, (*dns).dnsHeader.RCODE);
        printf("QDCOUNT = %x  \t┇  ANCOUNT = %x  \t┇  NSCOUNT = %x  \t┇  ARCOUNT = %x\n",
               (*dns).dnsHeader.QDCOUNT, (*dns).dnsHeader.ANCOUNT, (*dns).dnsHeader.NSCOUNT, (*dns).dnsHeader.ARCOUNT);
    }
}


void DNS_GetQuery(DNS *dns, int dnsLen) {
    if ((*dns).dnsHeader.QR == 0 && (*dns).dnsHeader.OPCODE == 0) {
        /* Question Section */
        printf("[request] ");

    } else {
        printf("[response] ");
    }

    // get QNAME
    (*dns).QNAMME = (*dns).buff + 12;
    char* URL = (char*) malloc(100);
    int len = 1, i = 0;
    int count = (*dns).QNAMME[0] - '\0';
    while ((*dns).QNAMME[len] != '\0') {
        if (count == 0) { URL[i] = '.'; count = (*dns).QNAMME[len] - '\0'; }
        else {URL[i] = (*dns).QNAMME[len]; count--; }
        len++; i++;
    }
    URL[i] = '\0';
    strcpy((*dns).QNAMME, URL);
    // get QTYPE
    (*dns).QTYPE = htons(*((*dns).QNAMME + 2));
    (*dns).QCLASS = htons(*((*dns).QNAMME + 4));

    free(URL);
    printf("%s\n\n", (*dns).QNAMME);
}


int main(int argc, char** argv) {
    if (strcmp(argv[1], "1") == 0) {
        DEBUGLEVEL = 1;     // 打印错误信息
    }
    if (strcmp(argv[1], "2") == 0) {
        DEBUGLEVEL = 2;     // 打印状态信息
    }
    if (strcmp(argv[1], "3") == 0) {
        DEBUGLEVEL = 3;     // 打印数据包
    }

    printf("---- DEBUG LEVEL = %d ----\n", DEBUGLEVEL);

    WSADATA wsadata;
    WSAStartup(MAKEWORD(2, 2), &wsadata);

    SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);

    SOCKADDR_IN localAddr;
    localAddr.sin_family = AF_INET;
    localAddr.sin_port = htons(53);
    localAddr.sin_addr.s_addr = inet_addr(ANY_ADDRESS);
    if (bind(sock, (SOCKADDR *) &localAddr, sizeof(localAddr)) == SOCKET_ERROR && DEBUGLEVEL >= 1) {
        printf("bind err");
    }
    SOCKADDR_IN remoteAddr;
    remoteAddr.sin_family = AF_INET;
    remoteAddr.sin_port = htons(53);
    remoteAddr.sin_addr.s_addr = inet_addr(LOCAL_ADDRESS);
    int lenRemoteAddr = sizeof(remoteAddr);

    SOCKADDR_IN fromAddr;
    int lenAnyAddr = sizeof(fromAddr);

    char buf[512];
    memset(&buf, '\0', sizeof(buf));
    int revLen;
    DNS dns;

    while (1) {
        revLen = recvfrom(sock, buf, sizeof(buf), 0, (SOCKADDR *) &fromAddr, &lenAnyAddr);
        if (revLen < 0 && DEBUGLEVEL >= 1) {
            printf("recv err");
        }

        if (revLen > 0) {
            if (DEBUGLEVEL >= 2) printf("接收到来自ip %s 的数据: ", inet_ntoa(fromAddr.sin_addr));

            memcpy(dns.buff, buf, revLen);
            DNS_GetHead(&dns);
            DNS_GetQuery(&dns, revLen);
        }
        revLen = sendto(sock, buf, revLen, 0, (SOCKADDR*)&remoteAddr, lenRemoteAddr);

        if (revLen < 0 && DEBUGLEVEL >= 1) {
            printf("send err");
            break;
        }
        memset(&dns, 0, sizeof(dns));
        memset(&buf, 0, sizeof(buf));
    }
}