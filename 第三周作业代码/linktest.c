#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>

void print_inode(const char *path) {
    struct stat st;
    if (lstat(path, &st) == -1) {
        perror("lstat失败");
        return;
    }
    printf("文件: %-20s | inode号: %lu\n", path, st.st_ino);
}

int main() {
    const char *source = "source.txt";
    const char *hardlink = "hardlink.txt";
    const char *softlink = "softlink.txt";

    // 创建源文件
    FILE *fp = fopen(source, "w");
    if (!fp) {
        perror("创建源文件失败");
        return 1;
    }
    fputs("这是源文件内容\n", fp);
    fclose(fp);

    // 创建硬链接
    if (link(source, hardlink) == -1) {
        perror("创建硬链接失败");
        if (errno == EEXIST) printf("硬链接已存在\n");
        return 1;
    }

    // 创建符号链接
    if (symlink(source, softlink) == -1) {
        perror("创建符号链接失败");
        if (errno == EEXIST) printf("符号链接已存在\n");
        return 1;
    }

    // 打印inode信息
    print_inode(source);
    print_inode(hardlink);
    print_inode(softlink);

    return 0;
}