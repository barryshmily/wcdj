include ../../../src/Inc.mk

INCLUDE = -I../comm/
LIBPATH = -L../lib/

DEP_LIBS = 
LIBS = -ltlpicomm $(DEP_LIBS)
OBJS = svshm_create.o

BIN_SVR = svshm_create 
BINS = $(BIN_SVR)

all: $(BINS)

$(BIN_SVR):$(OBJS)
	$(CXX) -o $@ $^ $(LIBPATH) $(LIBS)
	@echo "build $(BIN_SVR) OK!"

install:
	@echo "nothing to install"

clean:
	-rm -f $(OBJS) $(BINS) 



