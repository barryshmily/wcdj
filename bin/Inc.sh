# Inc.sh 
# public bash file

# you should set yourself path
OS_VERSION=`uname -v | grep "Darwin" |  awk -F" " '{print $1}'`
if [ "$OS_VERSION" == "Darwin" ]; then
	# OS X
	PROJECT_HOME="/Users/gerryyang/github_project/wcdj"
else
	# Linux
	PROJECT_HOME="/data/home/gerryyang/test/code_in_action/wcdj-master"
fi

CLIENT_WORKER_NUM=5

CLIENTSVMQKEY=10088
#SERVERSVMQKEY=10093
SERVERSVMQKEY=10108

PROC_PARAS="-projecthome=$PROJECT_HOME \
	-clientsvmqkey=$CLIENTSVMQKEY \
	-serversvmqkey=$SERVERSVMQKEY"

#DATE=`date "+%Y-%m-%d %H:%M:%S"`
DATE=`date "+%F %T"`

