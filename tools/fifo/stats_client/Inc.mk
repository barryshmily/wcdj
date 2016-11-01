
include ../../Inc.mk

CC     = gcc
CXX    = g++
RANLIB = ranlib
AR     = ar
AROPT  = -scurv

DATE = date "+%Y/%m/%d-%H:%M:%S"
os_date = $(shell date "+%Y%m%d-%H%M%S")

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

BUILD = BUILD_COMMON
export BUILD

ifeq ($(BUILD), BUILD_COMMON)
CFLAGS = -Werror -g -O2 -pipe -D_REENTRANT -fPIC -lz -lpthread -lrt -DNCS_WITH_SHM_AND_BOOST_SUPPORT
endif

DEPS_PATH  =  $(HOME_PATH)/deps
BOOST_PATH =  $(DEPS_PATH)/boost
NCS_PATH   =  $(PROJ_PATH)/src/common/include/protocol

INCLUDE += -I$(NCS_PATH) -I$(BOOST_PATH)

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

