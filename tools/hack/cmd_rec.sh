#!/bin/bash

# CMD used mostly recently

# set -o option-name
# history Enable command history, as described above under HISTORY. This option is on by default in interactive shells.
HISTFILE=~/.bash_history
set -o history
history | awk '{cmd[$4]++;count++}END{for(a in cmd)print cmd[a]" "cmd[a]/count*100"% "a}' | grep -v "./" | column -c3 -s" " -t | sort -nr | nl | head -n 10

#history | awk '{print $4}' | sort | uniq -c | sort -nrk 1 | head


