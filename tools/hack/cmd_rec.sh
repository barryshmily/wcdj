#!/bin/bash

# CMD used mostly
#cmd_used_most=`history | awk '{cmd[$4]++;count++}END{for(a in cmd)print cmd[a]" "cmd[a]/count*100"% "a}' | grep -v "./" | column -c3 -s" " -t | sort -nr | nl | head -n 10`
history | awk '{cmd[$4]++;count++}END{for(a in cmd)print cmd[a]" "cmd[a]/count*100"% "a}' | grep -v "./" | column -c3 -s" " -t | sort -nr | nl | head -n 10
#cmd_used_most_r=`history | awk '{print $4}' | sort | uniq -c | sort -nrk 1 | head`

echo "$cmd_used_most"
echo "$cmd_used_most_r"



