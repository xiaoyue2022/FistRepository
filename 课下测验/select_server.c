#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>

#define PORT 8080
#define MAX_CLIENTS 10

int main() {
    int listenfd, connfd, maxfd, sockfd;
    struct sockaddr_in servaddr, cliaddr;
    socklen_t clilen;
    char buffer[1024];
    int client[MAX_CLIENTS];
    fd_set readfds, allfds;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    listen(listenfd, 5);

    for (int i = 0; i < MAX_CLIENTS; i++) client[i] = -1;

    FD_ZERO(&allfds);
    FD_SET(listenfd, &allfds);
    maxfd = listenfd;

    while (1) {
        readfds = allfds;
        int nready = select(maxfd + 1, &readfds, NULL, NULL, NULL);
        if (nready < 0) {
            perror("select error");
            break;
        }

        if (FD_ISSET(listenfd, &readfds)) {
            clilen = sizeof(cliaddr);
            connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen);
            printf("新连接：%d\n", connfd);
            int i;
            for (i = 0; i < MAX_CLIENTS; i++) {
                if (client[i] < 0) {
                    client[i] = connfd;
                    break;
                }
            }
            FD_SET(connfd, &allfds);
            if (connfd > maxfd) maxfd = connfd;
            if (--nready <= 0) continue;
        }

        for (int i = 0; i < MAX_CLIENTS; i++) {
            sockfd = client[i];
            if (sockfd < 0) continue;
            if (FD_ISSET(sockfd, &readfds)) {
                int n = read(sockfd, buffer, sizeof(buffer));
                if (n <= 0) {
                    close(sockfd);
                    FD_CLR(sockfd, &allfds);
                    client[i] = -1;
                } else {
                    write(sockfd, buffer, n); // Echo 回客户端
                }
                if (--nready <= 0) break;
            }
        }
    }

    return 0;
}