#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define PORT 6543

void selective(int sd, struct sockaddr_in client) {
    int size = 4, curr = 0;
    char buff[128];
    socklen_t len = sizeof(client);

    for (int i = 0; i < size; i++) {
        recvfrom(sd, buff, sizeof(buff), 0, (struct sockaddr *)&client, &len);
        printf("RECV << %s\n", buff);
    }

    while (1) {
        if (curr == 4) {
            sprintf(buff, "%dNACK", curr);
            sendto(sd, buff, strlen(buff), 0, (struct sockaddr *)&client, len);
            printf("SEND >> %s\n", buff);
            recvfrom(sd, buff, sizeof(buff), 0, (struct sockaddr *)&client, &len);
            printf("RECV << %s\n", buff);
        }
        sprintf(buff, "%dACK", curr);
        sendto(sd, buff, strlen(buff), 0, (struct sockaddr *)&client, len);
        printf("SEND >> %s\n", buff);
        recvfrom(sd, buff, sizeof(buff), 0, (struct sockaddr *)&client, &len);
        printf("RECV << %s\n", buff);
        curr++;
    }
}

int main() {
    int sd;
    struct sockaddr_in server, client;

    if ((sd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        printf("Socket creation error.\n");
        return -1;
    }

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sd, (struct sockaddr *)&server, sizeof(server)) < 0) {
        printf("Binding error.\n");
        return -1;
    }

    selective(sd, client);

    close(sd);

    return 0;
}

