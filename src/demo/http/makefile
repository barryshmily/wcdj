include ../../Inc.mk

INCLUDE = -I../../modules/http/
LIBPATH = -L../../../lib/ -L../../dep/curl-7.34.0/lib/suse32_gcc412/


DEP_LIBS = -lrt -lz
LIBS = -lhttp -lcurl $(DEP_LIBS)
OBJS = http_demo.o

BIN_SVR = http_demo


BINS = $(BIN_SVR)


all: $(BINS)

$(BIN_SVR):$(OBJS)
	$(CXX) -o $@ $^ $(LIBPATH) $(LIBS)
	@echo "build $(BIN_SVR) OK!"
	#@cp $(BINS) ../../../bin -f

install:
	@echo "nothing to install"

clean:
	rm -f $(OBJS) $(BINS)

