#!/bin/bash

# calc tcp status
netstat -an | awk '/^tcp/{s[$6]++}END{for(i in s)print s[i]"\t"i}'

:<< wcdj
gerryyang@mba:hack$sh calc_tcp_status.sh 
8	LISTEN
11	CLOSE_WAIT
9	ESTABLISHED
wcdj

