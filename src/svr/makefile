include ../Inc.mk

INCLUDE = -I../../inc -I../comm

LIBPATH = -L../../lib

DEPS = $(patsubst %.o, .%.d, $(OBJS))

LIBS = -lsvqueue -lcommtools

OBJS = CServer.o main.o


ifeq ($(BUILD), BUILD_DEBUG_CLIENT)
BIN_SVR = wcdj_client 
endif

ifeq ($(BUILD), BUILD_DEBUG_SERVER)
BIN_SVR = wcdj_svr 
endif

BINS = $(BIN_SVR)


all: $(BINS)

$(BIN_SVR):$(OBJS)
	$(CXX) -o $@ $^ $(LIBPATH) $(LIBS)
	@echo "build $(BIN_SVR) OK!"
	@cp $(BINS) ../../bin/ -f

install:
	@echo "nothing to install"

clean:
	rm -f $(OBJS) wcdj*

