#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>


#define MAX 80
#define SA struct sockaddr
#define SAI struct sockaddr_in

void chat(int connfd) {
    int num = 0, con_num = 0;
    while (1) {
        con_num = 0;
        read(connfd, &con_num, sizeof(con_num));
        printf("Client: %d\nServer: ", ntohl(con_num));
        scanf("%d", &num);
        con_num = htonl(num);
        write(connfd, &con_num, sizeof(con_num));
        if (ntohl(con_num) == 0) {
            printf("Server Exit...\n");
            break;
        }
    }
}

int main() {
    SAI server, client;

    int sockfd, connfd, len, port;

    printf("Enter Port Number: ");
    scanf("%d", &port);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    printf("Socket successfully created\n");

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(port);

    if ((bind(sockfd, (SA *) &server, sizeof(server))) == 0)
        printf("Socket successfully binded\n");

    if ((listen(sockfd, 5)) == 0)
        printf("Server listening..\n");

    len = sizeof(client);
    connfd = accept(sockfd, (SA *) &client, (socklen_t * ) & len);
    printf("Server accept the client...\n");

    chat(connfd);
    close(sockfd);
    return 0;
}

