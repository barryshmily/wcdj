# for suse32_gcc412

include ../Inc.mk

DEP_PATH =  ../../dep

CURL_API_PATH    =  $(DEP_PATH)/curl-7.34.0
OPENSSL_API_PATH =  $(DEP_PATH)/openssl-1.0.1g

LIBPATH  =  -L../../lib/ \
			-L$(CURL_API_PATH)/lib \
			-L$(OPENSSL_API_PATH)/lib

INCLUDE += -I$(CURL_API_PATH)/include \
		   -I$(OPENSSL_API_PATH)/include

DEP_LIBS =  -lrt -lz -ldl
LIBS     =  -lhttp -lcurl -lssl -lcrypto $(DEP_LIBS)
OBJS     =  http_demo.o

BIN_SVR = http_demo


BINS = $(BIN_SVR)


all: $(BINS)

$(BIN_SVR):$(OBJS)
	$(CXX) -o $@ $^ $(LIBPATH) $(LIBS)
	@echo "build $(BIN_SVR) OK!"

install:
	@echo "nothing to install"

clean:
	rm -f $(OBJS) $(BINS)

