#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
    struct hostent *host;

    if (argc != 2) {
        fprintf(stderr, "用法: %s 域名\n", argv[0]);
        exit(1);
    }

    host = gethostbyname(argv[1]);
    if (host == NULL) {
        herror("gethostbyname error");
        exit(1);
    }

    printf("域名：%s\n", argv[1]);
    printf("IP地址：%s\n", inet_ntoa(*(struct in_addr *)host->h_addr));

    return 0;
}
