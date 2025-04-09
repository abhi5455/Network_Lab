#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define MAX 80
#define SA struct sockaddr
#define SAI struct sockaddr_in

void chat(int sockfd, SAI server) {
    printf("\nClient ready....\n");
    char buff[MAX];
    int n, len = sizeof(server);
    while (1) {
        bzero(buff, sizeof(buff));
        printf("\nClient:");
        n = 0;
        while ((buff[n++] = getchar()) != '\n');
        sendto(sockfd, buff, sizeof(buff), 0, (SA *) &server, len);
        bzero(buff, sizeof(buff));
        recvfrom(sockfd, buff, sizeof(buff), 0, (SA *) &server, (socklen_t * ) & len);
        printf("\nServer:%s", buff);
        if (strncmp(buff, "exit", 4) == 0) {
            printf("Client Exit...\n");
            break;
        }
    }

}

int main() {
    SAI server;
    int sockfd, port;

    printf("Enter Port Number: ");
    scanf("%d", &port);
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    printf("Socket successfully created\n");

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(port);

    chat(sockfd, server);
    close(sockfd);
    return 0;
}

