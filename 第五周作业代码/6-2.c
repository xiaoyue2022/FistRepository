#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void intHandler(int signo) {
    printf("SIGINT处理中...\n");
    sleep(2); // 模拟长时间处理
    printf("SIGINT处理完成\n");
}

void quitHandler(int signo) {
    printf("SIGQUIT处理中...\n");
    sleep(3);
    printf("SIGQUIT处理完成\n");
}

int main() {
    signal(SIGINT, intHandler);
    signal(SIGQUIT, quitHandler);

    while (1) {
        printf("等待输入...\n");
        pause(); // 等待信号
    }
    return 0;
}