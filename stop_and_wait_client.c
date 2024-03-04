#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>


struct pkt {
	int ACK;
	int seqno;
	char data[25];
};

int sockfd, counter, recvLen;
struct timeval t ={2,0};

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
	
	while(1)
	{
		printf("\nEnter data: ");
		scanf(" %s",sendPkt.data);
		sendPkt.ACK =0;
		sendPkt.seqno = counter;
		sendto(sockfd, &sendPkt, sizeof(sendPkt), 0, (struct sockaddr*)&addr, addrSize);
		setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (struct timeval*)&t, sizeof(t));
		
		while(1)
		{
			recvLen = recvfrom(sockfd, &recvPkt, sizeof(recvPkt), 0, (struct sockaddr*)&addr, &addrSize);
			if (recvLen>=0)
			{
				printf("\nACK recieved for packet no. %d \n", recvPkt.seqno);
					counter+=1;
					break;		
			}
			else
			{
				printf("\nTimeout! Pkt will be resent\n");
				sendto(sockfd, &sendPkt, sizeof(sendPkt), 0, (struct sockaddr*)&addr, addrSize);
			}		
		}	
	}
}

/*

OUTPUT

Enter data: Hello

ACK recieved for packet no. 0 

Enter data: hi hello

ACK recieved for packet no. 1 

Enter data: 
Timeout! Pkt will be resent

ACK recieved for packet no. 2 

Enter data: isac john

Timeout! Pkt will be resent

ACK recieved for packet no. 3 

Enter data: 
Timeout! Pkt will be resent

ACK recieved for packet no. 4 

*/
