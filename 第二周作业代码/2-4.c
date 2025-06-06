/**
 * @brief 主函数，程序的入口点
 * 
 * 此函数是程序的起始点，会初始化圆周率，输出问候语和 sin(π/2) 的值，最后返回 0 表示程序正常结束。
 */
#include <stdio.h>
#include <math.h>
 int main() {
    // 定义一个双精度浮点型变量 pi，并初始化为 3.1415926，用于近似表示圆周率
    double pi = 3.1415926;
    // 调用 printf 函数输出字符串 "hello world" 并换行
    printf("hello world\n");
    // 调用 printf 函数输出 "sin(pi/2) = " 以及 sin(pi/2) 的值，%f 是输出浮点数的格式说明符
    printf("sin(pi/2) = %f\n", sin(pi/2));
    // 返回 0 表示程序正常结束
    return 0;
}