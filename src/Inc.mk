CC     = gcc
CXX    = g++
RANLIB = ranlib
AR     = ar
AROPT  = -scurv

DATE = date "+%Y/%m/%d-%H:%M:%S"
os_date = $(shell date "+%Y%m%d-%H%M%S")

# you should change to yourself path
PROJECT_HOME = /data/home/gerryyang/test/code_in_action/proj_test

# check gcc version
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
#	BUILD:	编译类型
#------------------------------------------------------------------------------
#		BUILD_DEV:		    开发版本
#		BUILD_DEBUG:		测试版本
#		BUILD_NORMAL:		一般开发版本
#		BUILD_RELEASE:		发行版本
#------------------------------------------------------------------------------

BUILD = BUILD_RELEASE

ifeq ($(BUILD), BUILD_DEV)
CFLAGS = -Wall -g -DDEBUG
endif
ifeq ($(BUILD), BUILD_DEBUG)
CFLAGS = -Wall -g -DDEBUG
endif
ifeq ($(BUILD), BUILD_NORMAL)
CFLAGS = -Wall -g -O -DREENTRANT
endif
ifeq ($(BUILD), BUILD_RELEASE)
CFLAGS = -Werror -g -O2 -pipe -DNOPRINT_TERMINAL -DMY_DATE=\""`$(DATE)`"\"
#CFLAGS = -Werror -g -O2 -pipe -DMY_DATE=\""`$(DATE)`"\"
endif


# 自动计算文件的依赖关系
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

