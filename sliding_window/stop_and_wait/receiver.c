#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define MAX 1000
#define SA struct sockaddr
#define SAI struct sockaddr_in

int main() {
    SAI server, client;
    char buff[MAX], ack[] = "ack", nack[] = "no";
    int sockfd, connfd, len, count = 0, port;

    printf("Enter Port Number: ");
    scanf("%d", &port);
    getchar();  // flush newline

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sockfd, (SA *) &server, sizeof(server)) != 0) {
        perror("Bind failed");
        close(sockfd);
        exit(1);
    }

    listen(sockfd, 5);
    len = sizeof(client);
    connfd = accept(sockfd, (SA *) &client, (socklen_t * ) & len);
    if (connfd < 0) {
        perror("Accept failed");
        close(sockfd);
        exit(1);
    }

    printf("Receiver accepted the sender\n");

    bzero(buff, MAX);
    read(connfd, buff, sizeof(buff));
    int no = atoi(buff);
    printf("Expecting %d messages...\n", no);

    int received_count = 0;
    while (received_count < no) {
        bzero(buff, MAX);
        read(connfd, buff, sizeof(buff));
        printf("Received message: %s", buff);

        if ((strcmp(buff, "3\n") == 0 || strcmp(buff, "5\n") == 0) && count == 0) {
            sleep(3);
            write(connfd, nack, sizeof(nack));
            printf("Sending NACK\n");
            count = 1;
        } else {
            write(connfd, ack, sizeof(ack));
            printf("Sending ACK\n");
            received_count++;
            count = 0;
        }
    }

    close(sockfd);
    return 0;
}
