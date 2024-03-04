#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
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
	char *hello;
	char s[20],buffer[MAXLINE];
	time_t t;
	time(&t);
	while(1)
	{
		printf("\nenter data to send : ");
		gets(s);
		if(strcmp(s,"exit")==0)
			break;
		hello=s;
		struct sockaddr_in servaddr;
		if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) 
		{ 
			perror("socket creation failed"); 
			exit(EXIT_FAILURE);
		}
		memset(&servaddr, 0, sizeof(servaddr));
		servaddr.sin_family = AF_INET; 
		servaddr.sin_port = htons(PORT); 
		servaddr.sin_addr.s_addr = INADDR_ANY;
		int n, len;
		sendto(sockfd, (const char *)hello, strlen(hello), MSG_CONFIRM, (const struct sockaddr *) &servaddr,sizeof(servaddr)); 
		printf("Data sent to the Server.\n");
		n = recvfrom(sockfd, (char *)buffer, MAXLINE,MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
		buffer[n] = '\0';
		printf("Response from server : %s\n", buffer);
	}
	close(sockfd);
	return 0;
}

/*
OUTPUT

stud@debian:~/ije/udp$ ./udpc

enter data to send : Hello 
Data sent to the Server.
Response from server : Server Acknowledged

*/
