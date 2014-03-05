#!/bin/bash

:<< WCDJ_COMMENT
This script is used to monitor System V MQ health status 
WCDJ_COMMENT



# init your private paras

ur_msgid=1605676
# the size of msg is 15K
ur_msgsize=15360
# reaching this percent will notify
ur_percent=0.9



# fetch system paras

#msgmnb: max size of a message queue
#msgmnb=$(/sbin/sysctl -a | grep "kernel.msgmnb" | gawk -F'=' '{print $2}')
msgmnb=$(cat /proc/sys/kernel/msgmnb)
#echo $msgmnb

#msgmax: max size of message (bytes)
#msgmax=$(/sbin/sysctl -a | grep "kernel.msgmax" | gawk -F'=' '{print $2}')
msgmax=$(cat /proc/sys/kernel/msgmax)
#echo $msgmax

#msgmni:  max # of msg queue identifiers
#msgmni=$(/sbin/sysctl -a | grep "kernel.msgmni" | gawk -F'=' '{print $2}')
#msgmni=$(cat /proc/sys/kernel/msgmni)
msgmni=`cat /proc/sys/kernel/msgmni`
#echo $msgmni


# fetch current msgqueue status and check it's health

while [ 1 ]; do

	msgnum=$(ipcs -q -i $ur_msgid | grep "qnum" | gawk -F' ' '{print $3}' | gawk -F'=' '{print $2}')
	#echo $msgnum


	# calc the max msg number in current msgqueue
	# such as, msgmnb(16M) / ur_msgsize(15K) = 1092
	ur_max_msgnum=`expr $msgmnb / $ur_msgsize`
	#echo $ur_max_msgnum


	# monitor system status
	threshold=$(echo "$ur_max_msgnum * $ur_percent" | bc)
	#echo $threshold

	# test
	#msgnum=1000

	if [ `echo "$msgnum > $threshold" | bc` -eq 1 ]; then
		echo "System V MQ msgid $ur_msgid maybe full, current msgnum is $msgnum, and the biggest size is $ur_max_msgnum"
	else
		echo "System V MQ msgid $ur_msgid is OK, current msgnum is $msgnum, and the biggest size is $ur_max_msgnum"
	fi

	# sleep for a while
	sleep 1
done


