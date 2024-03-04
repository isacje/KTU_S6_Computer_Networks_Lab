#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <time.h>
#define PORT 8081
#define MAXLINE 1024
int main()
{
	int sockfd;
	char buffer [MAXLINE];
	time_t t;
	time(&t);
	char *hello = ctime(&t);
	char *ack= "Server Acknowledged";
	struct sockaddr_in servaddr, cliaddr;
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 )
	{ 
		perror("socket creation failed");                                 
		exit(EXIT_FAILURE);
	}
	memset(&servaddr, 0, sizeof(servaddr));
	memset(&cliaddr, 0, sizeof(cliaddr));
	servaddr.sin_family = AF_INET;                                          
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(PORT);
	if ( bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 )
	{
		perror("bind failed"); 
		exit(EXIT_FAILURE);
	}
	while(1)
	{
		int len, n;
		len = sizeof(cliaddr);                                      
		n = recvfrom(sockfd, (char *)buffer, MAXLINE,MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len);
		buffer[n] = '\0';
		printf("\nMsg from client : %s\n",buffer);
		if(strcmp(buffer,"time")==0)
		{
			sendto(sockfd, (const char *)hello, strlen(hello),MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);
			printf("TIME sent to the Client\n\n");
		}
		else
		{
			sendto(sockfd, (const char *)ack, strlen(ack),MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);
			printf("Acknowledgement sent to Client\n\n");
		}
	}
	return 0;
}

/*
OUTPUT

stud@debian:~/ije/udp$ ./udps

Msg from client : Hello
Acknowledgement sent to Client

*/
