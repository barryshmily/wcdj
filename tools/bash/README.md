
# 1 交互命令

## 1.1 Bash shortcuts

命令 | 作用
:----|:----
CTRL-c |  Stop current command
CTRL-z |  Sleep program
CTRL-a |  Go to start of line
CTRL-e |  Go to end of line 
CTRL-u |  Cut from start of line
CTRL-k |  Cut to end of line
CTRL-r |  Search history
!!     |  Repeat last command
!abc   |  Run last command starting with abc
!abc:p |  Print last command starting with abc
!$     |  Last argument of previous command
!*     |  All arguments of previous command
^abc^123 | Run previous command, replacing abc with 123

## 1.2 Bash Variables

环境变量 | 含义
:----|:----
env | Show environment variables
echo $NAME | Output value of $NAME variable
export NAME=value | Set $NAME to value
$PATH | Executable search path
$HOME | Home directory
$SHELL | Current shell

## 1.3 IO Redirection

命令 | 作用
:----|:----
cmd < file | Input of cmd from file
cmd1 < cmd2 | Output of cmd2 as file input to cmd1
cmd > file | Standard output (stdout) of cmd to file
cmd > /dev/null | Discard stdout of cmd
cmd >> file | Append stdout to file
cmd 2>file | Error output (stderr) of cmd to file
cmd 1>&2 | stdout to same place as stderr
cmd 2>&1 | stderr to same place as stdout
cmd &>file | every output of cmd to file

## 1.4 Search Files

命令 | 作用
:----|:----
grep pattern files | Search for pattern in files
grep -i | Case insensitive search
grep -r | Recursive search
grep -v | Inverted search
grep -o | Show matched part of file only
find /dir/ -name name* | Find files starting with name in dir
find /dir/ -user name | Find files owned by name in dir name
find /dir/ -mmin num | Find files modified less than num minutes ago in dir
whereis command | Find binary/source/manual for command
locate file | Find file (quick search of system index)

## 1.5 File Operations

命令 | 作用
:----|:----
touch file1 | Create file1
cat file1 file2 | Concatenate files and output
less file1 | View and paginate file1
file file1 | Get type of file1
cp file1 file2 | Copy file1 to file2
mv file1 file2 | Move file1 to file2
rm file1 | Delete file1
head file1 | Show first 10 lines of file1
tail file1 | Show last 10 lines of file1
tail -f file1 | Output last lines of file1 as it changes

## 1.6 Pipes

命令 | 作用
:----|:----
cmd1 \| cmd2 | stdout of cmd1 to cmd2
cmd1 \|& cmd2 | stderr of cmd1 to cmd2

## 1.7 Command Lists

命令 | 作用
:----|:----
cmd1;cmd2 | Run cmd1 then cmd2
cmd1 && cmd2 | Run cmd2 if cmd1 is successful
cmd1 \|\| cmd2 | Run cmd2 if cmd1 is not successful
cmd& | Run cmd in a subshell


*Refer*

[http://cheatography.com/davechild/cheat-sheets/linux-command-line/](http://cheatography.com/davechild/cheat-sheets/linux-command-line/)



# 2 必备技巧

作为Linux开发人员，经常需要和bash打交道，把经常执行的命令写成bash脚本可以极大地提高工作效率。下面总结一些常用的使用方法，方便使用的时候查阅。

## 2.1 如何对bash脚本进行传参

Arguments are accessed inside a script using the variables $1, $2, $3, etc., where $1 refers to the first argument, $2 to the second argument, and so on.

If you have a variable number of arguments, you can use the "$@" variable, which is an array of all the input parameters. This means you can use a for-loop to iteratively process each one.

``` bash
#!/bin/bash

for args in "$@"
do
   echo $args
done

echo "end"
```

If an argument has spaces, you need to enclose it with single quotes.

Frequently a script is written such that the user can pass in arguments in any order using flags. With the flags method, you can also make some of the arguments optional.

``` bash
#!/bin/bash

while getopts u:d:p:f: option
do
 case "${option}"
 in
 u) USER=${OPTARG}; echo $USER;;
 d) DATE=${OPTARG}; echo $DATE;;
 p) PRODUCT=${OPTARG}; echo $PRODUCT;;
 f) FORMAT=$OPTARG; echo $FORMAT;;
 esac
done

echo "end"
```

*Refer*

[How to Pass Arguments to a Bash-Script](https://www.lifewire.com/pass-arguments-to-bash-script-2200571)



## 2.2 top

` 显示每个CPU信息
-H 查看线程信息
id  CPU空闲百分比，压测时到达20-30基本认为负载已满


























