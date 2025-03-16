#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>

int main(int argc, char **argv) {
    const char *source = "source.txt";
    const char *symlink_path = "symlink_to_source.txt";
    struct stat source_stat, symlink_stat;
    char target_path[256];
    ssize_t len;

    // 创建源文件（如果不存在）
    FILE *fp = fopen(source, "w");
    if (!fp) {
        perror("创建源文件失败");
        return 1;
    }
    fputs("这是源文件内容\n", fp);
    fclose(fp);

    // 创建符号链接
    if (symlink(source, symlink_path) == -1) {
        perror("创建符号链接失败");
        if (errno == EEXIST) printf("符号链接已存在\n");
        return 1;
    }

    // 获取源文件属性（stat）
    if (stat(source, &source_stat) == -1) {
        perror("stat失败");
        return 1;
    }

    // 获取符号链接自身属性（lstat）
    if (lstat(symlink_path, &symlink_stat) == -1) {
        perror("lstat失败");
        return 1;
    }

    // 打印 inode 信息
    printf("源文件: %s | inode号: %lu\n", source, source_stat.st_ino);
    printf("符号链接: %s | inode号: %lu\n", symlink_path, symlink_stat.st_ino);

    // 读取符号链接指向的路径
    len = readlink(symlink_path, target_path, sizeof(target_path)-1);
    if (len == -1) {
        perror("readlink失败");
        return 1;
    }
    target_path[len] = '\0';
    printf("符号链接指向: %s\n", target_path);

    return 0;
}