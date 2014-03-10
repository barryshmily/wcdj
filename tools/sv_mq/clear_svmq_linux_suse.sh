#!/bin/bash

:<< WCDJ_COMMENT
This script is used to clear SystemV Message Queue for Linux SUSE.
WCDJ_COMMENT

username="gerryyang"

svmq_num=`ipcs -q  | grep "0x" | grep "$username" | awk -F' ' '{print $1}' | wc -l`
svmq_info=`ipcs -q | grep "0x" | grep "$username" | awk -F' ' '{print $1}' | xargs echo`
echo "svmq info($svmq_num): $svmq_info"


if [ $svmq_num -eq 0 ]; then
	echo "noting to clear"
else
	count=1
	ipcs -q | grep "0x" | grep "$username" | awk -F' ' '{print $1}' | while read line; do
		echo "clear $line SVMQ-keys" 
		#ipcrm -Q $line
		count=`expr $count + 1`
	done
fi

echo "clear over"
