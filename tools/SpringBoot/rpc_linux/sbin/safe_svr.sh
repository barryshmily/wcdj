#!/bin/sh

pname=com.tencent.rpc.App

function Start()
{
	pid=`ps -ef|grep $pname |grep -v grep|awk '{print $2}'`
	if [ "X$pid" = "X" ]; then
		sh run.sh com.tencent.rpc.App -c ../conf/config.properties &
		sleep 1
		pid=`ps -ef|grep $pname|grep -v grep|awk '{print $2}'`
		echo $pid > run.pid
		echo "START UP [$pname]" 
	else
		ps -ef|grep $pname|grep -v grep|awk '{print $2}'|xargs kill -1
		sleep 1	
		pid1=`ps -ef|grep $pname |grep -v grep|awk '{print $2}'`

		if [ "X$pid1" = "X" ]; then 	
			echo "KILL PROCESS"
			sh run.sh com.tencent.rpc.App -c ../conf/config.properties &
			echo "RESTART PROCESS OK" 
			pid2=`ps -ef|grep $pname |grep -v grep|awk '{print $2}'`
			echo $pid2 > run.pid
		else
			echo "PROCESS ALREADY EXIT,STOP FAILED"
		fi
	fi
}

function Stop()
{
	pid=`ps -ef|grep $pname |grep -v grep|awk '{print $2}'`
	if [ "X$pid" = "X" ]; then
		echo "NO RUN [$pname]" 
	else
		ps -ef|grep $pname|grep -v grep|awk '{print $2}'|xargs kill -1
		sleep 1	
		pid1=`ps -ef|grep $pname |grep -v grep|awk '{print $2}'`

		if [ "X$pid1" = "X" ]; then 	
			echo "KILL PROCESS OK"
		else
			echo "STOP FAILED"
		fi
	fi
}

function Restart()
{
	Start
}

if [[ "$1" == "start" ]]; then
	Start
elif [[ "$1" == "stop" ]]; then
	Stop
elif [[ "$1" == "restart" ]]; then
	Restart
else
	echo "$DATE usage: $0 start|stop|restart"
	exit 1
fi

exit 0


