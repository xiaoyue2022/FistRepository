#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void sigHandler(int signo) {
    printf("Received signal: %d\n", signo);
    // 重新注册信号处理函数（防止旧版本signal重置为默认行为）
    signal(SIGINT, sigHandler);
}

int main() {
    // 注册SIGINT处理函数
    signal(SIGINT, sigHandler);
    // 忽略SIGQUIT
    signal(SIGQUIT, SIG_IGN);

    while (1) {
        sleep(1); // 保持进程运行
    }
    return 0;
}