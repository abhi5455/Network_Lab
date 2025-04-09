#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define MAX 1000
#define SA struct sockaddr
#define SAI struct sockaddr_in

int main() {
    SAI server;
    char buff[MAX], ack[MAX];
    int sockfd, port, num_msgs;

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

    if (connect(sockfd, (SA *) &server, sizeof(server)) != 0) {
        perror("Connection with the server failed");
        close(sockfd);
        exit(1);
    }
    printf("Connected with server..\n");

    printf("Enter the number of messages: ");
    scanf("%d", &num_msgs);
    getchar();  // flush newline

    sprintf(buff, "%d", num_msgs);
    write(sockfd, buff, sizeof(buff));

    int sent_count = 0;
    while (sent_count < num_msgs) {
        bzero(buff, MAX);
        printf("Enter data for message %d: ", sent_count + 1);
        fgets(buff, MAX, stdin);

        write(sockfd, buff, sizeof(buff));

        bzero(ack, MAX);
        read(sockfd, ack, sizeof(ack));

        printf("Server reply: %s\n", ack);

        if (strcmp(ack, "ack") == 0) {
            sent_count++;
        } else {
            printf("Resending message...\n");
        }
    }

    close(sockfd);
    return 0;
}
