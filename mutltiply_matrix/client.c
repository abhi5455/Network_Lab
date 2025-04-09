#include<stdio.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<string.h>

#define MAX 50
#define SA struct sockaddr
#define SAI struct sockaddr_in
#define BUFFER_SIZE 1024

int main() {
    SAI server;
    int sockfd, connfd, len, port, m, n, p, q, multPossible;

    printf("Enter Port Number: ");
    scanf("%d", &port);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    printf("Socket created Successfully...\n");

    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    int A[MAX][MAX], B[MAX][MAX], result[MAX][MAX];

    printf("Enter the no of rows and columns of Matrix A: ");
    scanf("%d %d", &m, &n);
    printf("Enter the elements of matrix A:\n");
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            scanf("%d", &A[i][j]);
        }
    }

    printf("Enter the no of rows and columns of Matrix A: ");
    scanf("%d %d", &p, &q);
    printf("Enter the elements of matrix B:\n");
    for (int i = 0; i < p; i++) {
        for (int j = 0; j < q; j++) {
            scanf("%d", &B[i][j]);
        }
    }

    len = sizeof(server);
    sendto(sockfd, &m, sizeof(m), 0, (SA*) &server, len);
    sendto(sockfd, &n, sizeof(n), 0, (SA*) &server, len);
    sendto(sockfd, A, sizeof(A), 0, (SA*) &server, len);

    sendto(sockfd, &p, sizeof(m), 0, (SA*) &server, len);
    sendto(sockfd, &q, sizeof(n), 0, (SA*) &server, len);
    sendto(sockfd, B, sizeof(B), 0, (SA*) &server, len);

    recvfrom(sockfd, &multPossible, sizeof(multPossible), 0, (SA*) &server, &len);
    if(multPossible == 0){
        printf("Multiplication is not possible\n");
        return -1;
    }

    recvfrom(sockfd, result, sizeof(result), 0, (SA*) &server, &len);

    printf("The resultant matrix is: \n");
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < q; j++) {
            printf("%d ", result[i][j]);
        }
        printf("\n");
    }
    close(sockfd);
    return 0;
}
