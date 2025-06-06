#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main() {
    int fd = open("a.txt", O_RDWR);
    if (fd == -1) {
        perror("open failed");
        return 1;
    }

    // 移动指针到字符串末尾（第14字节）
    off_t oldpos = lseek(fd, strlen("abcdefghijklml"), SEEK_SET);
    if (oldpos == -1) {
        perror("lseek failed");
        close(fd);
        return 1;
    }

    // 写入新内容
    ssize_t result = write(fd, "uuuuu", strlen("uuuuu"));
    if (result == -1) {
        perror("write failed");
    } else {
        printf("Successfully wrote %zd bytes\n", result);
    }

    close(fd);
    return 0;
}