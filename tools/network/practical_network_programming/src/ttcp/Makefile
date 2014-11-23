
CC = gcc
CXX = g++
RANLIB = ranlib
AR = ar
AROPT = -scurv

CFLAGS = -Werror -g -Os -pipe -D_REENTRANT

BOOST = /Users/gerryyang/LAMP/boost_1_57_0/install/boost_1_57_0

INCLUDE = -I$(BOOST)/include
LIBPATH = $(BOOST)/lib/libboost_program_options.a

DEP_LIBS = 
LIBS = $(DEP_LIBS)


OBJS = common.o ttcp_blocking.o main.o
BIN = ttcp_blocking 

all: $(BIN)

$(BIN): $(OBJS)
	$(CXX) -o $@ $^ $(LIBPATH) $(LIBS)
	@echo "build $(BIN) OK!"

install:
	@echo "nothing to install"

clean:
	-rm -f $(OBJS) $(BIN)


%.o: %.cpp
	$(CXX) $(CFLAGS) $(INCLUDE) -c $<

%.o: %.cc
	$(CC) $(CFLAGS) $(INCLUDE) -c $<

