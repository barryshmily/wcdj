# Inc_proxy.sh 
# public bash file

# you should set yourself path
OS_VERSION=`uname -v | grep "Darwin" |  awk -F" " '{print $1}'`
if [ $OS_VERSION == "Darwin" ]; then
	# OS X
	PROJECT_HOME="/Users/gerryyang/github_project/wcdj"
else
	# Linux
	PROJECT_HOME="/data/home/gerryyang/test/code_in_action/wcdj-master"
fi

CLIENT_WORKER_NUM=1

CLIENTSVMQKEY=10088
SERVERSVMQKEY=10089

# set the number of proxy, if you set PROXY_NUM to 2, then the calculation is like below:
# client -> (10088) proxy1 (10089) -> (10089) proxy1 (10090) -> server
# You can use command [ipcs -q] to check the SV-MQs are created that is right or not.
PROXY_NUM=20
PROXY_BIN="proxy/bin/"

#DATE=`date "+%Y-%m-%d %H:%M:%S"`
DATE=`date "+%F %T"`
