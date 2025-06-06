#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

int main() {
    char hostname[256];
    struct hostent *host;

    if (gethostname(hostname, sizeof(hostname)) == -1) {
        perror("gethostname error");
        exit(1);
    }

    printf("主机名：%s\n", hostname);

    host = gethostbyname(hostname);
    if (host == NULL) {
        herror("gethostbyname error");
        exit(1);
    }

    printf("IP地址：%s\n", inet_ntoa(*(struct in_addr *)host->h_addr));

    return 0;
}
