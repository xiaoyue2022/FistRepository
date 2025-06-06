#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

void handler(int signo) {
    printf("PID %d 处理信号: %d\n", getpid(), signo);
}

int main() {
    struct sigaction act;
    act.sa_handler = handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    sigaction(SIGINT, &act, NULL);

    pid_t pid = fork();
    if (pid == 0) {
        printf("子进程 PID: %d\n", getpid());
        // execlp("ls", "ls", "-l", NULL); // 取消注释以测试exec后的行为
        while (1);
    } else {
        printf("父进程 PID: %d\n", getpid());
        while (1);
    }
    return 0;
}