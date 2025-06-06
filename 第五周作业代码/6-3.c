#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void handler(int signo, siginfo_t *info, void *context) {
    if (signo == SIGINT)
        printf("收到不可靠信号 SIGINT\n");
    else
        printf("收到可靠信号 SIGRTMIN\n");
}

int main() {
    struct sigaction act;
    sigset_t block_mask;

    // 设置信号处理函数
    act.sa_sigaction = handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_SIGINFO;

    sigaction(SIGINT, &act, NULL);
    sigaction(SIGRTMIN, &act, NULL);

    // 阻塞信号以模拟排队
    sigemptyset(&block_mask);
    sigaddset(&block_mask, SIGINT);
    sigaddset(&block_mask, SIGRTMIN);
    sigprocmask(SIG_BLOCK, &block_mask, NULL);

    printf("PID: %d\n", getpid());
    sleep(10); // 在此期间发送多个信号

    // 解除阻塞，处理排队信号
    sigprocmask(SIG_UNBLOCK, &block_mask, NULL);

    while (1); // 保持运行
    return 0;
}