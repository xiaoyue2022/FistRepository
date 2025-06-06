#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main() {
    int fd[2];
    pid_t pid;
    char buffer[100];
    const char* data = "[20242918][nkk]";

    // 创建管道
    if (pipe(fd) == -1) {
        perror("pipe");
        return 1;
    }

    // 创建子进程
    pid = fork();
    if (pid == -1) {
        perror("fork");
        return 1;
    } else if (pid == 0) {  // 子进程
        close(fd[1]);  // 关闭写端
        int bytes_read = read(fd[0], buffer, sizeof(buffer));
        if (bytes_read == -1) {
            perror("read");
            return 1;
        }
        buffer[bytes_read] = '\0';
        printf("子进程读取到的数据: %s\n", buffer);
        close(fd[0]);  // 关闭读端
    } else {  // 父进程
        close(fd[0]);  // 关闭读端
        if (write(fd[1], data, strlen(data)) == -1) {
            perror("write");
            return 1;
        }
        close(fd[1]);  // 关闭写端
        printf("父进程已写入数据\n");
    }

    return 0;
}
