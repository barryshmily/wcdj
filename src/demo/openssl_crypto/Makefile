include ../../Inc.mk

DEP_PATH = ../../dep
OPENSSL_API_PATH = $(DEP_PATH)/openssl/osx64_gcc421/openssl-1.0.1g

INCLUDE = -I../../modules/http \
		  -I$(OPENSSL_API_PATH)/include

LIBPATH = -L../../../lib/ \
		  -L$(OPENSSL_API_PATH)/lib 


DEP_LIBS = 
LIBS     = -lcrypto $(DEP_LIBS)

OBJS    = des_demo.o
BIN_SVR = des_demo

BINS = $(BIN_SVR)


all: $(BINS)

$(BIN_SVR):$(OBJS)
	$(CXX) -o $@ $^ $(LIBPATH) $(LIBS)
	@echo "build $(BIN_SVR) OK!"

install:
	@echo "nothing to install"

clean:
	rm -f $(OBJS) $(BINS)

