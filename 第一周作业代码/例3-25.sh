#!/bin/bash
# 定义一个名为 max 的函数，用于找出三个数中的最大值
function max () {
    # 检查传入的参数数量是否不等于 3
    if [ $# -ne 3 ]; then
        # 如果参数数量不等于 3，输出使用说明
        echo "usage: max p1 p2 p3"
        # 以状态码 1 退出函数，表示出现错误
        return 1
    fi
    # 假设第一个参数为最大值
    local max_num=$1
    # 检查第二个参数是否大于当前最大值
    if [ $max_num -lt $2 ]; then
        # 如果是，则将第二个参数赋值给最大值变量
        max_num=$2
    fi
    # 检查第三个参数是否大于当前最大值
    if [ $max_num -lt $3 ]; then
        # 如果是，则将第三个参数赋值给最大值变量
        max_num=$3
    fi
    # 输出最大值
    echo $max_num
}

# 调用 max 函数，传入参数 1, 2, 3，并将结果存储在变量 result 中
result=$(max 1 2 3)
# 检查函数调用是否成功（返回状态码为 0）
if [ $? -eq 0 ]; then
    # 输出最大值
    echo "the max number of 1 2 3 is: $result"
else
    echo "Function call failed."
fi