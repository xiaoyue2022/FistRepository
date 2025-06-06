#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 将数字模式转换为字符串形式（如 "-rwxr-xr-x"）
void mode_to_letter(mode_t mode, char *str) {
    strcpy(str, "----------"); // 初始化字符串

    // 判断文件类型
    if (S_ISDIR(mode))  str[0] = 'd';  // 目录
    if (S_ISCHR(mode))  str[0] = 'c';  // 字符设备
    if (S_ISBLK(mode))  str[0] = 'b';  // 块设备
    if (S_ISFIFO(mode)) str[0] = 'p';  // 管道
    if (S_ISLNK(mode))  str[0] = 'l';  // 符号链接
    if (S_ISSOCK(mode)) str[0] = 's';  // Socket

    // 解析权限位
    if (mode & S_IRUSR) str[1] = 'r';  // 用户读
    if (mode & S_IWUSR) str[2] = 'w';  // 用户写
    if (mode & S_IXUSR) str[3] = 'x';  // 用户执行

    if (mode & S_IRGRP) str[4] = 'r';  // 组读
    if (mode & S_IWGRP) str[5] = 'w';  // 组写
    if (mode & S_IXGRP) str[6] = 'x';  // 组执行

    if (mode & S_IROTH) str[7] = 'r';  // 其他用户读
    if (mode & S_IWOTH) str[8] = 'w';  // 其他用户写
    if (mode & S_IXOTH) str[9] = 'x';  // 其他用户执行
}

// 读取文件属性并打印
void do_ls(const char *pathname) {
    struct stat attr;
    char mode[11];

    if (stat(pathname, &attr) == -1) {
        perror("stat");
        exit(EXIT_FAILURE);
    }

    mode_to_letter(attr.st_mode, mode);
    printf("文件属性: %s | 链接数: %ld | 所有者: %d | 大小: %ld 字节\n",
           mode, (long)attr.st_nlink, attr.st_uid, (long)attr.st_size);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "用法: %s <文件名>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    do_ls(argv[1]);
    return 0;
}