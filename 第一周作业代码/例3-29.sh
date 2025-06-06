#!/bin/bash
trap"echo a=Sa b=$b"EXIT#EXIT信号是程序执行exit命令时产生的信号
a=20 b=40
exit