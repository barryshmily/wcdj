#!/bin/bash

SERVER=tcpcli
CLIENT_NUM=20
PROC_PARAS=9002

function Stop()
{
	killall -9 tcpcli
}

function Start()
{
	i=1
	while [ $i -le $CLIENT_NUM ]; do
		echo "start client$i"
		Boot
		i=`expr $i + 1`
	done
}

function Boot()
{
	./$SERVER $PROC_PARAS &
	echo "$SERVER starts ok"
}

ulimit -c unlimited

if [ "$1" == "start" ]; then
	Start
elif [ "$1" == "stop" ]; then
	Stop
else
	echo "usage: $0 start|stop"
	exit 0
fi

exit 0

