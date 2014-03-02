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
PROJECT_HOME = /data/home/gerryyang/test/code_in_action/wcdj-master

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
# BUILD Mode:
#
# System V MQ Client:
# BUILD_DEBUG_CLIENT
# BUILD_RELEASE_CLIENT
#
# System V MQ Server:
# BUILD_DEBUG_SERVER
# BUILD_RELEASE_SERVER
#------------------------------------------------------------------------------
# MACRO OPTIONS:
#
# [1] NOPRINT_TERMINAL: do not print info on terminal, a.k.a close fd 1 2 3.
# [2] CLIENT_MODE/SERVER_MODE: one mode used only, to test IPC like System V IPC.
#
#==============================================================================

#BUILD = BUILD_DEBUG_CLIENT
BUILD = BUILD_DEBUG_SERVER
export BUILD

ifeq ($(BUILD), BUILD_DEBUG_CLIENT)
CFLAGS = -Werror -g -O2 -pipe -DCLIENT_MODE -DMY_DATE=\""`$(DATE)`"\"
endif
ifeq ($(BUILD), BUILD_DEBUG_SERVER)
CFLAGS = -Werror -g -O2 -pipe -DSERVER_MODE -DMY_DATE=\""`$(DATE)`"\"
endif
ifeq ($(BUILD), BUILD_RELEASE_CLIENT)
CFLAGS = -Werror -g -O2 -pipe -DNOPRINT_TERMINAL -DCLIENT_MODE -DMY_DATE=\""`$(DATE)`"\"
endif
ifeq ($(BUILD), BUILD_RELEASE_SERVER)
CFLAGS = -Werror -g -O2 -pipe -DNOPRINT_TERMINAL -DSERVER_MODE -DMY_DATE=\""`$(DATE)`"\"
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

