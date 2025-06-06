#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s [-s] source target\n", argv[0]);
        return 1;
    }
    
    if (strcmp(argv[1], "-s") == 0) {
        if (symlink(argv[2], argv[3]) == -1) {
            perror("symlink failed");
            return 1;
        }
    } else {
        if (link(argv[1], argv[2]) == -1) {
            perror("link failed");
            return 1;
        }
    }
    return 0;
}
