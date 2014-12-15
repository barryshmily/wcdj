#!/bin/bash

# find static ip
#strings portal_dispatch | grep -oP  '(?:\d{1,3}\.){3}\d{1,3}'
echo "1.1.1.1" | egrep '(?:\d{1,3}\.){3}\d{1,3}'
echo "255.255.255.255" | grep -E '(?:\d{1,3}\.){3}\d{1,3}'

:<< wcdj
gerryyang@mba:hack$sh find_static_ip_from_bin.sh 
1.1.1.1
255.255.255.255
wcdj
