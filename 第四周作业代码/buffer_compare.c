#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

#define BUFFER_SIZE 4096

int main() {
    // 系统调用（内核缓冲）
    int sys_fd = open("syscall_copy.txt", O_WRONLY | O_CREAT, 0644);
    clock_t start = clock();
    for (int i = 0; i < 100000; i++) {
        write(sys_fd, "A", 1); // 每次1字节，触发100000次系统调用
    }
    close(sys_fd);
    printf("系统调用耗时: %f秒\n", (double)(clock() - start) / CLOCKS_PER_SEC);

    // 标准I/O（用户缓冲）
    FILE *stdio_fp = fopen("stdio_copy.txt", "w");
    start = clock();
    for (int i = 0; i < 100000; i++) {
        fputc('A', stdio_fp); // 用户缓冲自动批量写入
    }
    fclose(stdio_fp);
    printf("标准I/O耗时: %f秒\n", (double)(clock() - start) / CLOCKS_PER_SEC);
    
    return 0;
}