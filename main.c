#include "include.h"
#include "struct.h"
#include "hashmap.h"

// ��ȡDNSͷ����Ϣ
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
}

// ��ȡDNSͷ����Ϣ
void DNS_GetQuery(DNS *dns) {
    // Question Section
    // get QNAME
    memcpy((*dns).dnsQueries.QNAMME, (*dns).buff + 12, sizeof((*dns).dnsQueries.QNAMME));
    char* URL = (char*) malloc(100);
    int len = 1, i = 0;
    int count = (*dns).dnsQueries.QNAMME[0] - '\0';
    while ((*dns).dnsQueries.QNAMME[len] != '\0') {
        if (count == 0) { URL[i] = '.'; count = (*dns).dnsQueries.QNAMME[len] - '\0'; }
        else {URL[i] = (*dns).dnsQueries.QNAMME[len]; count--; }
        len++; i++;
    }
    URL[i] = '\0';
    strcpy((*dns).dnsQueries.QNAMME, URL);
    // get QTYPE
    (*dns).dnsQueries.QTYPE = (uint16_t)*((*dns).buff + 12 + len + 2);
    (*dns).dnsQueries.QCLASS = (uint16_t)*((*dns).buff + 12 + len + 4);
    free(URL);
}

// ��ȡDNS����Ϣ
void DNS_GetAnswer(DNS *dns) {

}

// ��ȡDNS��Ȩ��Ϣ
void DNS_GetAuthority(DNS *dns) {

}

// ��ȡDNS������Ϣ
void DNS_GetAdditional(DNS *dns) {

}


// ��ӡDNS��Ϣ
void DNS_Print(DNS *dns) {
    if (DEBUGLEVEL >= 2) {
        if ((*dns).dnsHeader.QR == 0 && (*dns).dnsHeader.OPCODE == 0 && DEBUGLEVEL >= 2) {
            /* Question Section */
            printf("[request] \n");

        } else {
            printf("[response] \n");
        }
    }
    if (DEBUGLEVEL >= 3) {
        printf("## DNS Header: (Hexadecimal)\n");
        printf("ID = %x  \t��  QR = %x  \t��  OP = %x  \t��  AA = %x\n",
               (*dns).dnsHeader.ID, (*dns).dnsHeader.QR, (*dns).dnsHeader.OPCODE, (*dns).dnsHeader.AA);
        printf("TC = %x  \t��  RD = %x  \t��  RA = %x  \t��  RC = %x\n",
               (*dns).dnsHeader.TC, (*dns).dnsHeader.RD, (*dns).dnsHeader.RA, (*dns).dnsHeader.RCODE);
        printf("QD = %x  \t��  AN = %x  \t��  NS = %x  \t��  AR = %x\n",
               (*dns).dnsHeader.QDCOUNT, (*dns).dnsHeader.ANCOUNT, (*dns).dnsHeader.NSCOUNT, (*dns).dnsHeader.ARCOUNT);

        printf("## DNS Question: (Hexadecimal)\n");
        printf("QNAME: %s  \n", (*dns).dnsQueries.QNAMME);
        printf("QTYPE: %x  \n", (*dns).dnsQueries.QTYPE);
        printf("QCLASS: %x  \n", (*dns).dnsQueries.QCLASS);

        printf("\n");
    }
}

int main(int argc, char** argv) {
    if (strcmp(argv[1], "1") == 0) {
        DEBUGLEVEL = 1;     // ��ӡ������Ϣ
    }
    if (strcmp(argv[1], "2") == 0) {
        DEBUGLEVEL = 2;     // ��ӡ״̬��Ϣ
    }
    if (strcmp(argv[1], "3") == 0) {
        DEBUGLEVEL = 3;     // ��ӡ���ݰ�
    }

    printf("---- DEBUG LEVEL = %d ----\n", DEBUGLEVEL);

    FILE *fp;
    if ((fp = fopen("dnsrelay.txt", "r")) == NULL) {
        if (DEBUGLEVEL >= 1)
            printf("Can't find the dnsrelay file.\n");
        exit(0);
    }//�Ѵ����txt�ĵ������ip-�������ձ��ȡ��
    char *url;
    url = (char *) malloc(255);
    char *ip;
    ip = (char *) malloc(255);
    int i = 0;//���������±�ֱ��ʾ��ǰ�����url��ip��λ������
    while (i != -1) {
        memset(url,0,255);
        memset(ip,0,255);//����������״̬�ع��ʼֵ
        if(fscanf(fp,"%s %s",ip,url)==-1)
            break;//�����ļ���һ�в����ո�ǰ���ֵ�ֱ𸳸�ip��url�������ȡʧ������ѭ��
        insertMap(url,ip);//�������url-ip
    }
    free(url);
    free(ip);

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
            if (DEBUGLEVEL >= 2) printf("���յ�����ip %s ������: \n", inet_ntoa(fromAddr.sin_addr));
            memcpy(dns.buff, buf, revLen);
            DNS_GetHead(&dns);
            DNS_GetQuery(&dns);
            DNS_Print(&dns);
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