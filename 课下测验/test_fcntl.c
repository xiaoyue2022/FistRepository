#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

// 测试 fcntl 获取和设置文件状态标志
void test_file_flags() {
    int fd = open("test_file.txt", O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        perror("open");
        return;
    }

    // 获取当前文件状态标志
    int flags = fcntl(fd, F_GETFL);
    if (flags == -1) {
        perror("fcntl F_GETFL");
        close(fd);
        return;
    }

    printf("Original file flags: %d\n", flags);

    // 设置新的文件状态标志
    flags |= O_APPEND;
    if (fcntl(fd, F_SETFL, flags) == -1) {
        perror("fcntl F_SETFL");
    } else {
        printf("File flags set to append mode.\n");
    }

    close(fd);
}

// 测试 fcntl 的文件锁功能
void test_file_lock() {
    int fd = open("test_file.txt", O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        perror("open");
        return;
    }

    struct flock lock;
    memset(&lock, 0, sizeof(lock));

    // 设置写锁
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;

    if (fcntl(fd, F_SETLKW, &lock) == -1) {
        perror("fcntl F_SETLKW");
    } else {
        printf("Write lock acquired.\n");
    }

    // 释放写锁
    lock.l_type = F_UNLCK;
    if (fcntl(fd, F_SETLK, &lock) == -1) {
        perror("fcntl F_SETLK");
    } else {
        printf("Write lock released.\n");
    }

    close(fd);
}

int main() {
    // 测试文件状态标志操作
    test_file_flags();

    // 测试文件锁操作
    test_file_lock();

    return 0;
}
