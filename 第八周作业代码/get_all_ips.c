#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

int main() {
    char hostname[256];
    struct hostent *host;
    int i = 0;

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

    while (host->h_addr_list[i] != NULL) {
        printf("IP地址 %d：%s\n", i + 1,
               inet_ntoa(*(struct in_addr *)host->h_addr_list[i]));
        i++;
    }

    return 0;
}
