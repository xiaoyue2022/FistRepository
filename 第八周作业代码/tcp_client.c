#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define SERVER_IP "127.0.0.1"
#define PORT 8888
#define BUFSIZE 1024

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUFSIZE] = "你好，服务器！";

    // 创建套接字
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket error");
        exit(1);
    }

    // 设置服务器地址
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    // 连接服务器
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect error");
        exit(1);
    }

    // 发送数据
    if (send(sock, buffer, strlen(buffer), 0) < 0) {
        perror("send error");
        exit(1);
    }

    printf("消息发送成功。\n");

    // 关闭套接字
    close(sock);

    return 0;
}
