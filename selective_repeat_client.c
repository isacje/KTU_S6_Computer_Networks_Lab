#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define PORT 6543
#define LOST "127.0.0.1"

void selective(int sd, struct sockaddr_in server) {
    char data[128][128], buff[128];
    int curr, size = 4, i;
    socklen_t len = sizeof(server);

    for (curr = 0; curr < size; curr++) {
        printf("SEND << ");
        scanf("%s", data[curr]);
        sendto(sd, data[curr], strlen(data[curr]) + 1, 0, (struct sockaddr *)&server, sizeof(server));
    }

    while (1) {
        recvfrom(sd, buff, sizeof(buff), 0, (struct sockaddr *)&server, &len);
        printf("RECV >> %s\n", buff);
        if (buff[1] == 'N') {
            sendto(sd, data[curr - size], strlen(data[curr - size]) + 1, 0, (struct sockaddr *)&server, sizeof(server));
            printf("RESEND: %s\n", data[curr - size]);
            continue;
        }
        printf("SEND << ");
        scanf("%s", data[curr]);
        sendto(sd, data[curr], strlen(data[curr]) + 1, 0, (struct sockaddr *)&server, sizeof(server));
        curr++;
    }
}

int main() {
    int sd;
    struct sockaddr_in server;

    if ((sd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        printf("Socket creation error.\n");
        return -1;
    }

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr(LOST);

    selective(sd, server);

    close(sd);

    return 0;
}

