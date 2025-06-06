#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
    struct hostent *host;
    struct in_addr addr;

    if (argc != 2) {
        fprintf(stderr, "用法: %s IP地址\n", argv[0]);
        exit(1);
    }

    inet_pton(AF_INET, argv[1], &addr);

    host = gethostbyaddr((const void *)&addr, sizeof(addr), AF_INET);
    if (host == NULL) {
        herror("gethostbyaddr error");
        exit(1);
    }

    printf("IP地址：%s\n", argv[1]);
    printf("主机名：%s\n", host->h_name);

    return 0;
}
