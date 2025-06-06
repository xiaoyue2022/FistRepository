#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void alarm_handler(int signo) {
    // 空处理，仅用于唤醒pause
}

unsigned int mysleep(unsigned int seconds) {
    signal(SIGALRM, alarm_handler);
    alarm(seconds);
    pause();
    return 0;
}

int main() {
    printf("开始睡眠\n");
    mysleep(3);
    printf("睡眠结束\n");
    return 0;
}