#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
    struct servent *serv;

    if (argc != 3) {
        fprintf(stderr, "用法: %s 服务名 协议\n", argv[0]);
        exit(1);
    }

    serv = getservbyname(argv[1], argv[2]);
    if (serv == NULL) {
        fprintf(stderr, "找不到服务: %s\n", argv[1]);
        exit(1);
    }

    printf("服务名：%s\n", argv[1]);
    printf("协议：%s\n", argv[2]);
    printf("端口号：%d\n", ntohs(serv->s_port));

    return 0;
}
