include ../../Inc.mk

INCLUDE = -I../../modules/fifo/
LIBPATH = -L../../../lib/ 

DEP_LIBS = -lpthread
LIBS = -lipcfifo $(DEP_LIBS)
OBJS = ipc_fifo_demo.o

BIN_SVR = ipc_fifo_demo 


BINS = $(BIN_SVR)


all: $(BINS)

$(BIN_SVR):$(OBJS)
	$(CXX) -o $@ $^ $(LIBPATH) $(LIBS)
	@echo "build $(BIN_SVR) OK!"
	#@cp $(BINS) ../../../bin -f

install:
	@echo "nothing to install"

clean:
	-rm -f $(OBJS) $(BINS) fifo_file



