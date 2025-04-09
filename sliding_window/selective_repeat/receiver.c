#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>

int is_corrupt() {
    return rand() % 3 == 0;
}

int main() {
    srand(time(0));

    struct sockaddr_in server = {0};
    char buff[50];
    int sockfd = socket(AF_INET, SOCK_STREAM, 0), port;

    printf("Enter Port Number: ");
    scanf("%d", &port);

    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    connect(sockfd, (struct sockaddr *) &server, sizeof(server));
    printf("Client started...\n");

    while (1) {
        bzero(buff, sizeof(buff));
        int n = read(sockfd, buff, sizeof(buff));
        if (n <= 0) break;

        printf("Received: %s\n", buff);
        char ack[20];
        int id = buff[strlen(buff) - 1] - '0';

        if (is_corrupt()) {
            sprintf(ack, "NACK-%d", id);
            printf("Simulating corruption... Sending: %s\n", ack);
        } else {
            sprintf(ack, "ACK-%d", id);
            printf("Message OK... Sending: %s\n", ack);
        }

        write(sockfd, ack, sizeof(ack));
    }

    printf("Client exiting...\n");
    close(sockfd);
    return 0;
}