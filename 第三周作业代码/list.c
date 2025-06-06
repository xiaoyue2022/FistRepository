#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>

int main(int ac, char **av) {
    DIR *dir_ptr;
    struct dirent *dir_entry;

    if (ac < 2) {
        printf("Usage: listdir directoryname\n");
        exit(1);
    }

    dir_ptr = opendir(av[1]);  // 打开目录
    if (dir_ptr == NULL) {
        perror("无法打开目录");
        exit(1);
    }

    while ((dir_entry = readdir(dir_ptr)) != NULL) {  // 逐项读取目录内容
        printf("%s\n", dir_entry->d_name);             // 打印文件名
    }

    closedir(dir_ptr);  // 关闭目录
    return 0;
}