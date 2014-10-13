# Inc.mk
CC     = gcc
CXX    = g++
RANLIB = ranlib
AR     = ar
AROPT  = -scurv
CFLAGS = -Werror -g -Os -pipe -D_REENTRANT

OPENSSL_PATH = /data/home/gerryyang/LAMP/openssl/install/openssl-1.0.1g

INCLUDE = -I$(OPENSSL_PATH)/include
LIBPATH = -L$(OPENSSL_PATH)/lib

DEP_LIBS = -lcrypto -lrt -lz -ldl
LIBS    = $(DEP_LIBS)

%.o: %.cpp
	$(CXX) $(CFLAGS) $(INCLUDE) -c $<
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $<



