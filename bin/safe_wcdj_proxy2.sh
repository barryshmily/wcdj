#!/bin/sh
# ================================================

# include public script

# [1] only for running scripts at current path
#source Inc.sh

# [2] make scripts all be relative to one another
#MY_DIR=`dirname $0`
#$MY_DIR/Inc.sh

# [3] An alternative to [2]
# The advantage being not having the dependence on dirname, 
# which is not a built-in command
#MY_DIR=${0%/*}
#$MY_DIR/Inc.sh

# [4] 
source $(dirname $0)/Inc2.sh

# ================================================


SERVER=wcdj_proxy2
SERVER_PATH=$PROJECT_HOME/bin
SERVER_CONF=$PROJECT_HOME/conf/$SERVER.conf
SERVER_PID=$PROJECT_HOME/bin/$SERVER.pid


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
    cd $SERVER_PATH; ./$SERVER $PROC_PARAS 
    echo "$DATE $SERVER starts OK" 
}

function Start()
{
  if [ "$PROC_ID" == "0" ]; then
  	# need to run server
    Boot

  else
	# error, server has started
  	echo "$DATE error:`basename "$PROC_ID"` already exist! use [$0 stop] first"
  fi
}

function Stop()
{
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
}

function Reboot()
{
  Stop
  sleep 2
  Start
}

# run from here

GetPid
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

