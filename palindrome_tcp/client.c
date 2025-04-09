#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define MAX 80
#define SA struct sockaddr
#define SAI struct sockaddr_in

void chat(int sockfd) {
    char buff[MAX];
    int n;
    while (1) {
        bzero(buff, sizeof(buff));
        printf("Client: ");
        n = 0;
        while ((buff[n++] = getchar()) != '\n');

        write(sockfd, buff, strlen(buff) + 1);

        if (strncmp(buff, "exit", 4) == 0) {
            printf("Client Exit...\n");
            break;
        }

        bzero(buff, sizeof(buff));
        read(sockfd, buff, sizeof(buff));
        printf("Server response: %s\n", buff);
    }
}

int main() {
    SAI server;
    int sockfd, port;

    printf("Enter Port Number: ");
    scanf("%d", &port);
    getchar();
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    printf("Socket successfully created\n");
    bzero(&server, sizeof(server));

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(port);
    if (connect(sockfd, (SA *) &server, sizeof(server)) == 0)
        printf("connected to the server...\n");

    chat(sockfd);
    close(sockfd);
    return 0;
}

