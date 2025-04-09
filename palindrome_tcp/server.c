#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <ctype.h>

#define MAX 80
#define SA struct sockaddr
#define SAI struct sockaddr_in

int is_palindrome(char str[]) {
    int len = strlen(str);
    for (int i = 0; i < len / 2; i++)
        if (str[i] != str[len - i - 1])
            return 0;
    return 1;
}

void reverse_string(char str[]) {
    int len = strlen(str);
    for (int i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = temp;
    }
}

void chat(int connfd) {
    char buff[MAX];
    int n;
    while (1) {
        bzero(buff, MAX);
        read(connfd, buff, sizeof(buff));
        buff[strcspn(buff, "\n")] = '\0';

        if (strncmp("exit", buff, 4) == 0) {
            printf("Server Exit...\n");
            break;
        }

        printf("Received from client: %s\n", buff);

        if (is_palindrome(buff)) {
            printf("%s is a palindrome\n", buff);
            strcpy(buff, "Palindrome");
        } else {
            printf("%s is not a palindrome. Reversing and sending back...\n", buff);
            reverse_string(buff);
        }

        write(connfd, buff, strlen(buff) + 1);
    }
}


int main(){
    SAI server,client;
    int sockfd, connfd, len, port;

    printf("Enter Port Number: ");
    scanf("%d", &port);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    printf("Socket successfully created\n");

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(port);
    if ((bind(sockfd, (SA*)&server, sizeof(server))) == 0) 
        printf("Socket successfully binded\n");
    if ((listen(sockfd, 5)) == 0) 
        printf("Server listening...\n");
    len = sizeof(client);
    connfd = accept(sockfd, (SA*)&client, (socklen_t*)&len);
    printf("Server accept the client\n");

    chat(connfd);
    close(sockfd);
    return 0;
}

