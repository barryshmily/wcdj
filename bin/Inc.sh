# Inc.sh 
# public bash file

PROJECT_HOME=/Users/gerryyang/github_project/wcdj

CLIENT_WORKER_NUM=60

CLIENTSVMQKEY=10088
SERVERSVMQKEY=10093

PROC_PARAS="-projecthome=$PROJECT_HOME \
	-clientsvmqkey=$CLIENTSVMQKEY \
	-serversvmqkey=$SERVERSVMQKEY"

#DATE=`date "+%Y-%m-%d %H:%M:%S"`
DATE=`date "+%F %T"`

