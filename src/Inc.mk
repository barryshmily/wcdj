#==============================================================================
# The common configure about Makefile
#==============================================================================

CC     = gcc
CXX    = g++
RANLIB = ranlib
AR     = ar
AROPT  = -scurv

DATE = date "+%Y/%m/%d-%H:%M:%S"
os_date = $(shell date "+%Y%m%d-%H%M%S")

# you should set yourself path
OS_VERSION = $(shell uname -v | grep "Darwin" |  awk -F" " '{print $$1}')
ifeq (${OS_VERSION},Darwin)
# OS X
PROJECT_HOME = /Users/gerryyang/github_project/wcdj
else
# Linux
PROJECT_HOME = /data/home/gerryyang/test/code_in_action/wcdj-master
endif

# check os and gcc version
gccver=$(shell gcc -v  2>&1 | grep "gcc version" | awk -F" " '{print $$3}')
os64bitstr=$(shell uname -a | grep "_64 ")
osbit=64
ifeq (${os64bitstr},)
	osbit=32
endif
releaseverstr=$(shell gcc -v  2>&1 | grep -i "slackware")
releasever=slack
ifeq ($(releaseverstr),)
	releasever=suse
endif
os_ver=${releasever}_${osbit}_${gccver}

export os_ver
export os_date

#==============================================================================
# BUILD OPTIONS:
#
# [1] NOPRINT_TERMINAL: do not print info on terminal, a.k.a close fd 1 2 3.
#                       Note: in order to test performance, or use [./safe_wcdj_client.sh start > /dev/null]
#
# [2] CLIENT_MODE/SERVER_MODE/PROXY_MODE: one mode used only, to test IPC like System V IPC.
#
# [3] BLOCK_ACCEPT: if you set this macro that svr uses block msgrcv to accept request. 
#                   In the opposite, do not set this macro that svr will accept request in non-block mode.
#                   Note that this macro is only used in SERVER_MODE.
#
#==============================================================================

#BUILD = BUILD_DEBUG_CLIENT
#BUILD = BUILD_DEBUG_SERVER
BUILD = BUILD_DEBUG_PROXY

export BUILD

ifeq ($(BUILD), BUILD_DEBUG_CLIENT)
CFLAGS = -Werror -g -O2 -pipe -DCLIENT_MODE -DMY_DATE=\""`$(DATE)`"\"
endif
ifeq ($(BUILD), BUILD_DEBUG_SERVER)
CFLAGS = -Werror -g -O2 -pipe -DSERVER_MODE -DBLOCK_ACCEPT -DMY_DATE=\""`$(DATE)`"\"
endif
ifeq ($(BUILD), BUILD_DEBUG_PROXY)
CFLAGS = -Werror -g -O2 -pipe -DPROXY_MODE -DBLOCK_ACCEPT -DMY_DATE=\""`$(DATE)`"\"
endif


# calc relation between files automatically
.%.d: %.cpp
	$(CC) $(INCLUDE) -MM $< > $@
	@$(CC) $(INCLUDE) -MM $< | sed s/"^"/"\."/  |  sed s/"^\. "/" "/  | \
                sed s/"\.o"/"\.d"/  >> $@
%.o: %.cpp 
	$(CXX) $(CFLAGS) $(INCLUDE) -c $<

.%.d: %.c
	$(CC) $(INCLUDE) -MM $< > $@
	@$(CC) $(INCLUDE) -MM $< | sed s/"^"/"\."/  |  sed s/"^\. "/" "/  | \
                sed s/"\.o"/"\.d"/  >> $@
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $<

