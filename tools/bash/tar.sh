#!/bin/bash

# 创建一个打包文件
tar cvf newarc.tar.gz tmp/a/b/* 

# 查看这个打包文件的内容, 输出tmp/a/b/c/
tar tf newarc.tar.gz

# 将当前目录的这个打包文件解压到任一目录下
tar xvf newarc.tar.gz -C tmp/a/b/c 


