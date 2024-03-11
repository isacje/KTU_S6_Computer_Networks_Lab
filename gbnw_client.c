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
};
int sockfd, counter, recvLen;
struct timeval t = {2, 0};
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
    int base = 0;
    int nextSeqNum = 0;
    struct pkt buffer[WINDOW_SIZE];
    while (1)
    {
        while (nextSeqNum < base + WINDOW_SIZE)
        {
            sendPkt.ACK = 0;
            sendPkt.seqno = nextSeqNum;
            buffer[nextSeqNum % WINDOW_SIZE] = sendPkt;
            sendto(sockfd, &sendPkt, sizeof(sendPkt), 0, (struct sockaddr *)&addr, addrSize);
            nextSeqNum++;
        }
        setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (struct timeval *)&t, sizeof(t));
        while (1)
        {
            recvLen = recvfrom(sockfd, &recvPkt, sizeof(recvPkt), 0, (struct sockaddr *)&addr, &addrSize);
            if (recvLen >= 0)
            {
                printf("\nACK received for packet no. %d \n", recvPkt.seqno);
                base = recvPkt.seqno + 1;
                break;
            }
            else
            {
                printf("\nTimeout! Resending packets from %d to %d\n", base, nextSeqNum - 1);
                int i;
                for (i = base; i < nextSeqNum; i++)
                    sendto(sockfd, &buffer[i % WINDOW_SIZE], sizeof(buffer[i % WINDOW_SIZE]), 0, (struct sockaddr *)&addr, addrSize);          
            }
        }
    }
}
