include ../Inc.mk

INCLUDE = -I../../inc

LIBPATH = -L../../lib

DEPS = $(patsubst %.o, .%.d, $(OBJS))

LIBS =

OBJS = CServer.o main.o

BIN_SVR = test_svr

BINS = $(BIN_SVR)


all:  $(BINS)

$(BIN_SVR):$(OBJS)
	$(CXX) -o $@ $^ $(LIBPATH) $(LIBS)

install:
	cp $(BINS) ../../bin/ -f

clean:
	rm -f $(OBJS) $(BINS)

