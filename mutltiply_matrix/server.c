#include<stdio.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<string.h>

#define MAX 50
#define SA struct sockaddr
#define SAI struct sockaddr_in
#define BUFFER_SIZE 1024

void multiply_matrix(int A[MAX][MAX], int B[MAX][MAX], int result[MAX][MAX], int m, int n) {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            result[i][j] = 0;
            for (int k = 0; k < m; k++) {
                result[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main() {
    int sockfd;
    SAI server, client;
    int len = sizeof(client), port, m, n, p, q, multPossible = 0;

    printf("Enter Port Number: ");
    scanf("%d", &port);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    printf("Socket created Successfully...\n");

    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(sockfd, (struct sockaddr *) &server, sizeof(server));
    printf("Socket binded Successfully\n");

    int A[MAX][MAX], B[MAX][MAX], result[MAX][MAX];

    recvfrom(sockfd, &m, sizeof(m), 0, (struct sockaddr *) &client, (socklen_t * ) & len);
    recvfrom(sockfd, &n, sizeof(n), 0, (struct sockaddr *) &client, (socklen_t * ) & len);
    recvfrom(sockfd, A, sizeof(A), 0, (struct sockaddr *) &client, (socklen_t * ) & len);
    printf("Matrix A: \n");
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d ", A[i][j]);
        }
        printf("\n");
    }

    recvfrom(sockfd, &p, sizeof(m), 0, (struct sockaddr *) &client, (socklen_t * ) & len);
    recvfrom(sockfd, &q, sizeof(n), 0, (struct sockaddr *) &client, (socklen_t * ) & len);
    recvfrom(sockfd, B, sizeof(B), 0, (struct sockaddr *) &client, (socklen_t * ) & len);
    printf("Matrix B: \n");
    for (int i = 0; i < p; i++) {
        for (int j = 0; j < q; j++) {
            printf("%d ", B[i][j]);
        }
        printf("\n");
    }

    if (p == q) {
        multPossible = 1;
        printf("Cannot perform Multiplication");
        sendto(sockfd, &multPossible, sizeof(multPossible), 0, (struct sockaddr *) &client, len);
        return -1;
    }

    multiply_matrix(A, B, result, m, q);
    sendto(sockfd, result, sizeof(result), 0, (struct sockaddr *) &client, len);

    printf("Multiplication Result is sent back to client\n");
    close(sockfd);
    return 0;
}
