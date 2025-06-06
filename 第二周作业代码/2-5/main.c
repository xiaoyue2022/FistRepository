#include <stdio.h> 
#include "calc.h"
int main(int argc, char* argv[])
{
    // 定义四个双精度浮点型变量，v1 和 v2 用于存储输入的数值，m 用于存储平均值，sum2 用于存储总和
    double v1, v2, m, sum2;
    // 初始化 v1 的值为 3.2
    v1 = 3.2;
    v2 = 8.9;
    // 调用 sum 函数计算 v1 和 v2 的总和，注意此时 v2 未初始化
    sum2 = sum(v1, v2);
    // 调用 aver 函数计算 v1 和 v2 的平均值
    m = aver(v1, v2);
    // 输出 v1 和 v2 的平均值，注意格式字符串中的错误，应将 83.2f 改为 %3.2f
    printf("The mean of %3.2f and %3.2f is %3.2f\n", v1, v2, m);
    // 输出 v1 和 v2 的总和，注意格式字符串中的错误，应将 83.2f 改为 %3.2f
    printf("The sum of %3.2f and %3.2f is %3.2f\n", v1, v2, sum2);

    // 返回 0 表示程序正常结束
    return 0;
}