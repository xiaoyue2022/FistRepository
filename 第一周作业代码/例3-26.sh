#!/bin/bash
# 定义一个名为 array 的数组，包含多个元素
array=(jerry tom alice keven julie)
# 初始化索引变量 index 为 0，用于遍历数组
index=0
# 开始一个 while 循环，判断 index 是否小于数组长度
while [ $index -lt ${#array[@]} ]; do
    # 输出当前索引对应的数组元素值
    echo "array[$index]=${array[$index]}"
    # 索引自增 1
    index=$(($index + 1))
done
# 输出数组的所有元素
echo "all array is ${array[*]}"
# 给数组 array2 的索引为 10 的位置赋值为 hello
array2[10]="hello"
# 给数组 array2 的索引为 20 的位置赋值为 world
array2[20]="world"
# 输出 array2 索引为 10 的元素值
echo "array2[10]=${array2[10]}"
# 输出 array2 索引为 15 的元素值，若未赋值则为空
echo "array2[15]=${array2[15]}"
# 输出 array2 索引为 20 的元素值
echo "array2[20]=${array2[20]}"
# 退出脚本
exit 0