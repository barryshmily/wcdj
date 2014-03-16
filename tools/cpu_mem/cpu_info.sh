#!/bin/bash

:<< WCDJ_COMMENT
This script is used to show some information about ur CPU,
Note that we assume the CPUs are all the same.
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

