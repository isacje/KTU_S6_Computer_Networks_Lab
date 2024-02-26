#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080
int main(int argc, char const* argv[])
{
	int server_fd, new_socket, valread, opt = 1;
	struct sockaddr_in address;
	char buffer[1024] = { 0 };
	int addrlen = sizeof(address);
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	setsockopt(server_fd, SOL_SOCKET,SO_REUSEADDR | SO_REUSEPORT, &opt,sizeof(opt));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);
	bind(server_fd, (struct sockaddr*)&address,sizeof(address));
	listen(server_fd, 3);
	while(1)
	{
			char hello[100]={0};
			char buffer[1024] = { 0 };
			new_socket= accept(server_fd, (struct sockaddr*)&address,(socklen_t*)&addrlen);
			valread= read(new_socket,buffer,1024);
			printf("%s received from client\n\n", buffer);
			printf("Enter message from server:");
			gets(hello);
			printf("\n");
			if(strcmp(hello,"exit")==0)
			{
				printf("\nServer closed\n");
				send(new_socket, hello, strlen(hello), 0);
				break;
			}
			else{
				send(new_socket, hello, strlen(hello), 0);
				printf("%s message sent to client\n\n",hello);
			}
	}
	close(new_socket);
	shutdown(server_fd, SHUT_RDWR);
	return 0;
}
