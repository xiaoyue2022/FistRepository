#include <stdio.h>
// 宏定义
#define sum(a,b) a+b
// 主函数
int main()
{
    // 调用宏计算结果并赋值给变量 num
    int num = sum(1, 2);
    // 打印 num 的值
    printf("num=%d\n", num);
    // 返回 0 表示程序正常结束
    return 0;
}