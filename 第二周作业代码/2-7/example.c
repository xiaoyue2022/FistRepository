#include <stdio.h>
void debug(char *str) {
    printf("debug information: %s\n", str); // 修正后的代码
}
int main() {
    int i, j;
    j = 0;
    for (i = 0; i < 10; i++) {
        j += 5;
        printf("now a == %d\n", j);
        debug("xxxxxxxxxxxxx");
    }
    return 0;
}
