#!/bin/bash
# 输出变量 x 的初始值，由于还未赋值，这里输出为空
echo $x
# 给变量 x 赋值为 1
x=1
# 开始一个 while 循环，当 x 的值小于等于 5 时，持续执行循环体
while [ "$x" -le 5 ]; do
    # 使用 bc 命令进行更灵活的数值计算，将 x 的值乘以 2 后重新赋值给 x
    x=$(echo "$x * 2" | bc)
    # 打印每次循环后 x 的值，方便调试查看
    echo "Current value of x: $x"
done
# 输出最终的 x 值
echo "Final value of x: $x"
# 退出脚本，返回状态码 0 表示脚本正常结束
exit 0