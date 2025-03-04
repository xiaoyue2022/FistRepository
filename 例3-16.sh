#!/bin/bash
# 输出提示信息，引导用户选择喜欢的水果
echo "please choose your favorite fruit:"
# 使用 select 语句创建一个菜单，用户可以从列表中选择一个选项
# 这里提供了三个选项：APPLE、ORANGE 和 BANANA
select fruit in "APPLE" "ORANGE" "BANANA"; do
    # 输出用户选择的水果名称
    # 注意：原代码中 $(fruit) 有误，应改为 ${fruit}
    echo "your favorite fruit is ${fruit}";
# 结束 select 循环
done