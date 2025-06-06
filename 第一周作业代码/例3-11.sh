#!/bin/bash
# 声明一个变量 x，并将其初始值设置为 0
x=0 
# 开始一个 while 循环，循环条件是 x 的值不等于 5
# 注意：原代码中 while 和 [ 之间缺少空格，应修正为 while [ "$x" -ne 5 ]; do
while["$x"-ne 5];do 
    # 打印当前 x 的值
    echo $x
    # 将 x 的值加 1，并将结果重新赋值给 x
    x=$(($x+1))
# 结束 while 循环
done
# 退出脚本，返回状态码 0 表示脚本正常结束
exit 0