#!/bin/sh
# ================================================
# include public script
#
# [1] only for running scripts at current path
#source Inc.sh
#
# [2] make scripts all be relative to one another
#MY_DIR=`dirname $0`
#$MY_DIR/Inc.sh
#
# [3] An alternative to [2]
# The advantage being not having the dependence on dirname, 
# which is not a built-in command
#MY_DIR=${0%/*}
#$MY_DIR/Inc.sh
#
# [4] 
source $(dirname $0)/Inc_proxy.sh
#
# ================================================

SERVER=wcdj_proxy
SERVER_BACKUP=wcdj_proxy
SERVER_PATH="$PROJECT_HOME/bin/"
SERVER_LINK_PATH="$PROJECT_HOME/bin/$PROXY_BIN"


# save pid into PROC_ID
# use CreatePIDFile function in code to create pid file 
function GetPid()
{
	if [ -f $SERVER_PID ]; then
		read PROC_ID < $SERVER_PID
	else
		PROC_ID=0
	fi
}

function Boot()
{
	#echo "start $SERVER_LINK_PATH$SERVER...$CLIENTSVMQKEY...$SERVERSVMQKEY"
	cd $SERVER_LINK_PATH; ./$SERVER "-projecthome=$PROJECT_HOME" \
		"-clientsvmqkey=$CLIENTSVMQKEY" \
		"-serversvmqkey=$SERVERSVMQKEY" > /dev/null

	RET=$?
	if [ $RET == "0" ]; then
		echo "$DATE $SERVER starts OK" 
	else
		echo "$DATE $SERVER starts Error[$RET]" 
	fi
}

function Start()
{
:<< COMMENT_WCDJ
	if [ "$PROC_ID" == "0" ]; then
		# need to run server
		Boot
	else
		# error, server has started
		echo "$DATE error:`basename "$PROC_ID"` already exist! use [$0 stop] first"
	fi
COMMENT_WCDJ

	# if you wanna create multi-instances
	i=1
	TMPSVMQKEY=""
	TMPSERVER=""
	while [ "$i" -le "$PROXY_NUM" ]; do

		TMPSERVER=$SERVER
		SERVER=$SERVER$i
		TMPSVMQKEY=$SERVERSVMQKEY

		ln -s "$SERVER_PATH/$SERVER_BACKUP" "$SERVER_LINK_PATH/$SERVER"
		Boot

		SERVER=$TMPSERVER
		CLIENTSVMQKEY=`expr "$TMPSVMQKEY"`
		SERVERSVMQKEY=`expr "$SERVERSVMQKEY" + 1`

		i=`expr "$i" + 1`
	done
}

function Stop()
{
:<< WCDJ_COMMENT
	if [ "$PROC_ID" == "0" ]; then
		echo "$DATE $SERVER is not running"
	else
		#kill -s TERM $PROC_ID
		kill -s USR2 $PROC_ID
		echo "$DATE $SERVER has stopped"
		# delete pid file
		rm $SERVER_PID
		# make sure that server has stopped
		sleep 0.2; killall -9 $SERVER >/dev/null 2>&1
	fi
WCDJ_COMMENT

	# stop multi-instances
	i=1
	TMPSERVER=""
	while [ $i -le $PROXY_NUM ]; do
		TMPSERVER="$SERVER_BACKUP$i"
		echo "stop $TMPSERVER"
		killall -9 $TMPSERVER >/dev/null 2>&1

		rm -f "$TMPSERVER.pid"
		rm -f "$SERVER_LINK_PATH/$TMPSERVER"

		sleep 0.2
		i=`expr $i + 1`
	done
}

function Reboot()
{
	Stop
	sleep 2
	Start
}

# run from here

#GetPid
ulimit -c unlimited

if [ "$1" == "start" ]; then
	Start

elif [ "$1" == "stop" ]; then
	Stop

elif [ "$1" == "reboot" ] || [ "$1" == "restart" ]; then
	Reboot

else
	echo "$DATE usage: $0 (start|stop|reboot|restart)"
	exit 1

fi

exit 0

