# 2012-04-27 gerryyang
# Inc.sh 
# public bash file

PROJECT_HOME=..

CLIENT_WORKER_NUM=1

CLIENTSVMQKEY=10088
SERVERSVMQKEY=10089

PROC_PARAS="-projecthome=$PROJECT_HOME \
	-clientsvmqkey=$CLIENTSVMQKEY \
	-serversvmqkey=$SERVERSVMQKEY"


#DATE=`date "+%Y-%m-%d %H:%M:%S"`
DATE=`date "+%F %T"`
