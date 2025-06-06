/* fcntltest.c */
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main() {
    int flags;
    int file_descriptor;
    char* text1 = "abcdefghij";
    char* text2 = "0123456789";
    char read_buffer[25];

    // 创建新文件并写入初始内容
    file_descriptor = creat("testfile", S_IRWXU);
    write(file_descriptor, text1, 10);
    close(file_descriptor);

    // 重新以读写模式打开文件
    file_descriptor = open("testfile", O_RDWR);
    read(file_descriptor, read_buffer, 24);
    printf("First read: '%s'\n", read_buffer);

    // 重置文件指针到开头
    lseek(file_descriptor, 0, SEEK_SET);

    // 设置追加模式
    flags = fcntl(file_descriptor, F_GETFL);   // 获取当前标志
    flags |= O_APPEND;                        // 添加追加标志
    fcntl(file_descriptor, F_SETFL, flags);   // 设置新标志

    // 尝试写入数据
    write(file_descriptor, text2, 10);

    // 再次读取并验证
    lseek(file_descriptor, 0, SEEK_SET);
    memset(read_buffer, '\0', 25);
    read(file_descriptor, read_buffer, 24);
    printf("Second read: '%s'\n", read_buffer);

    close(file_descriptor);
    unlink("testfile");
    return 0;
}