#!/bin/bash

:<< WCDJ_COMMENT
This script is used to show information of pc memory.
WCDJ_COMMENT

pid=24852

echo "--- mem info [beg] ---"
cat /proc/meminfo 
echo "--- mem info [end] ---"


echo "--- prog virtual mem info [beg] ---"
cat /proc/$pid/maps
echo "--- prog virtual mem info [end] ---"






