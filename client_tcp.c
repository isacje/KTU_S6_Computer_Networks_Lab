#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080

int main(int argc, char const* argv[])
{
	int status, valread, client_fd;
	struct sockaddr_in serv_addr;
	char hello[100];
	printf("Enter message from client:");
	gets(hello);
	while(strcmp(hello,"exit")!=0)
	{
		char buffer[1024] = { 0 };
		client_fd = socket(AF_INET, SOCK_STREAM, 0);
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_addr.s_addr= INADDR_ANY;
		serv_addr.sin_port = htons(PORT);
		status= connect(client_fd, (struct sockaddr*)&serv_addr,sizeof(serv_addr));
		send(client_fd, hello, strlen(hello), 0);
		printf("%s message sent to server\n\n",hello);
		valread = read(client_fd, buffer, 1024);
		if(strcmp(buffer,"exit")==0){
			printf("Server Terminated\n");
			break;
		}
		printf("%s received from server\n\n",buffer);
		printf("Enter message from client:");
		gets(hello);
	}
	close(client_fd);
	return 0;
}
