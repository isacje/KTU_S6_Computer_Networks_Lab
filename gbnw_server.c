#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#define WINDOW_SIZE 4
struct pkt {
    int ACK;
    int seqno;
    char data[25];
};
int sockfd, counter, recvLen;
int drop;
void main()
{
    struct pkt sendPkt, recvPkt;
    struct sockaddr_in addr;
    socklen_t addrSize;
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    addrSize = sizeof(addr);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(4000);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    bind(sockfd, (struct sockaddr *)&addr, addrSize);
    int expectedSeqNum = 0;
    struct pkt buffer[WINDOW_SIZE];
    int received[WINDOW_SIZE];
    int i;
    while (1)
    {
        recvLen = recvfrom(sockfd, &recvPkt, sizeof(recvPkt), 0, (struct sockaddr *)&addr, &addrSize);
        if (recvLen >= 0)
        {
            drop = rand() % 8;
            if (drop <= 1)
            {
                printf("\nPacket with no. %d received but no ACK will be sent to simulate lost ACK\n", recvPkt.seqno);
                continue;
            }
            else if (drop <= 2)
            {
                printf("\nPacket with no. %d received but will be dropped to simulate lost packet\n", recvPkt.seqno);
                continue;
            }
            else
            {
                if (recvPkt.seqno == expectedSeqNum)
                {
                    printf("\nPacket with no. %d received, sending ACK to simulate normal transmission\n", recvPkt.seqno);
                    sendPkt.ACK = 1;
                    sendPkt.seqno = recvPkt.seqno;
                    sendto(sockfd, &sendPkt, sizeof(sendPkt), 0, (struct sockaddr *)&addr, addrSize);
                    received[recvPkt.seqno % WINDOW_SIZE] = 1;
                    while (received[expectedSeqNum % WINDOW_SIZE])
                    {
                        expectedSeqNum++;
                        for (i = 0; i < WINDOW_SIZE; i++)
                        {
                            if (received[(expectedSeqNum + i) % WINDOW_SIZE])
                                received[(expectedSeqNum + i) % WINDOW_SIZE] = 0;
                            else
                                break;
                        }
                    }
                }
                else
                    printf("\nPacket with no. %d received, but out-of-order. Discarding.\n", recvPkt.seqno);
            }
        }
        else
            printf("\nAn error occurred\n");
    }
}
