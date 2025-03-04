#!/bin/bash
# 输出传递给脚本的所有参数
echo $*
# 输出上一个命令的退出状态
echo $?
# 输出传递给脚本的参数个数
echo $#
# 输出当前脚本的进程ID
echo $$
# 输出当前脚本的文件名
echo $0
# 输出当前用户的有效用户ID
echo $EUID
message="hello world!" # 定义一个打印显示的变量message
# 输出变量message的值
echo $message
# 退出脚本
exit