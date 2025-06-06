#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define PORT 8888
#define BACKLOG 5
#define BUFSIZE 1024

int main() {
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len;
    char buffer[BUFSIZE];
    int recv_len;

    // 创建套接字
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) {
        perror("socket error");
        exit(1);
    }

    // 配置地址结构
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // 绑定套接字
    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind error");
        exit(1);
    }

    // 监听
    if (listen(server_sock, BACKLOG) < 0) {
        perror("listen error");
        exit(1);
    }

    printf("服务器启动，等待连接...\n");

    // 接收连接
    addr_len = sizeof(client_addr);
    client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &addr_len);
    if (client_sock < 0) {
        perror("accept error");
        exit(1);
    }

    printf("客户端连接成功。\n");

    // 接收数据
    recv_len = recv(client_sock, buffer, BUFSIZE - 1, 0);
    if (recv_len < 0) {
        perror("recv error");
        exit(1);
    }

    buffer[recv_len] = '\0';
    printf("接收到客户端数据：%s\n", buffer);

    // 关闭套接字
    close(client_sock);
    close(server_sock);

    return 0;
}
