#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>  // 修正：type -> types
#include <fcntl.h>      // 修正：fcnt1 -> fcntl
#define BUFFERSIZE 512  // 修正：BUPFERSIZE512 -> BUFFERSIZE 512

void debug(char *mess, char *param, int n) {  // 添加大括号
    if(n == -1) {
        printf("Error occured: %s %s\n", mess, param);  // 修正：s s -> %s %s
        exit(1);
    }
}

int main(int ac, char **av) {  // 添加返回类型int和大括号
    int in_fd, out_fd, n_chars;
    char buf[BUFFERSIZE];
    
    if(ac != 3) {
        printf("usage: %s source destination\n", *av);  // 修正：in -> \n
        exit(1);
    }
    
    in_fd = open(av[1], O_RDONLY);  // 修正：:// -> ;
    debug("cannot open", av[1], in_fd);
    
    out_fd = creat(av[2], 0744);  // 修正：/创建 -> ;
    debug("Cannot creat", av[2], out_fd);
    
    while((n_chars = read(in_fd, buf, BUFFERSIZE)) > 0) {  // 修正：infd -> in_fd
        if(write(out_fd, buf, n_chars) != n_chars) {
            debug("Write error to", av[2], -1);
        }
    }
    
    close(in_fd);
    close(out_fd);  // 修正：fd)---1 -> fd
    return 0;  // 添加返回值
}