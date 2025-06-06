#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

int main() {
    const char *filename = "/root/secret.txt";
    int fd = open(filename, O_RDONLY);

    if (fd == -1) {
        // 直接输出错误码
        printf("错误码: %d\n", errno);
        
        // 使用perror输出描述
        perror("打开文件失败");
        
        // 使用strerror转换错误码
        printf("详细错误: %s\n", strerror(errno));
        
        // 针对性处理
        if (errno == EACCES) {
            fprintf(stderr, "权限不足，请使用sudo运行\n");
        } else if (errno == ENOENT) {
            fprintf(stderr, "文件不存在\n");
        }
    } else {
        printf("文件打开成功\n");
        close(fd);
    }
    
    return 0;
}