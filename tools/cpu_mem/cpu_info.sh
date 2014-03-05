#!/bin/bash

:<< WCDJ_COMMENT
This script is used to show some information about ur CPU,
Note that we assume the CPUs are all the same.

u also can use command [top] to see CPU info:

--- total control ---
h: show help about top command (Remark: very useful when u forget how to use), or use [man top].

Z,B       Global: 'Z' change color mappings; 'B' disable/enable bold
1,I       Toggle SMP view: '1' single/separate states; 'I' Irix/Solaris mode
f,o     . Fields/Columns: 'f' add or remove; 'o' change display order
F or O  . Select sort field (Remark: by default)
<,>     . Move sort field: '<' next col left; '>' next col right (Remark: very useful)
R,H     . Toggle: 'R' normal/reverse sort; 'H' show threads (Remark: very useful)
c,i,S   . Toggle: 'c' cmd name/line; 'i' idle tasks; 'S' cumulative time (Remark: very usefu)
x,y     . Toggle highlights: 'x' sort field; 'y' running tasks
z,b     . Toggle: 'z' color/mono; 'b' bold/reverse (only if 'x' or 'y') (Remark: very useful)
u       . Show specific user only (Remark: very useful)
n or #  . Set maximum tasks displayed (Remark: some useful)
k,r       Manipulate tasks: 'k' kill; 'r' renice
d or s    Set update interval
W         Write configuration file
q         Quit

--- first part control ---
Toggle Summaries: 'l' load avg; 't' task/cpu stats; 'm' mem info

l: show or hide first line info in the first part
t: show or hide second and thrid lines info in the first part
m: show or hide fourth and fifth lines info int the first part

--- thrid part control ---
N: show PID sorted by number
P: show CPU sorted by number
M: show MEM sorted by number

WCDJ_COMMENT


physical_cpu_num=$(cat /proc/cpuinfo | grep "physical id" | uniq | wc -l)
echo "physical CPUs: "$physical_cpu_num

logical_cpu_num=$(cat /proc/cpuinfo | grep "processor" | wc -l)
echo "logical CPUs: "$logical_cpu_num

cpu_core_num=$(cat /proc/cpuinfo | grep "cores" | uniq)
echo $cpu_core_num

cpu_hz=$(cat /proc/cpuinfo | grep "MHz" | uniq )
echo $cpu_hz

cpu_cachesize=$(cat /proc/cpuinfo | grep "cache size" | uniq)
echo $cpu_cachesize

echo "--- /proc/stat info [begin] ---"
cat /proc/stat
echo "--- /proc/stat info [end] ---"

