#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define MAX 80
#define SA struct sockaddr
#define SAI struct sockaddr_in

void send_file(int sockfd) {
    char data[MAX] = {0};
    char filename[100];

    printf("Enter filename: ");
    scanf("%s", filename);

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("File open failed");
        exit(1);
    }

    while (fgets(data, MAX, fp) != NULL) {
        send(sockfd, data, strlen(data), 0);
        bzero(data, MAX);
    }

    fclose(fp);
}

int main() {
    SAI server;
    int sockfd, port;

    printf("Enter Port Number: ");
    scanf("%d", &port);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    printf("Socket successfully created..\n");
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(port);

    if (connect(sockfd, (SA *) &server, sizeof(server)) == 0)
        printf("connected to the server..\n");

    send_file(sockfd);
    close(sockfd);
    return 0;
}
