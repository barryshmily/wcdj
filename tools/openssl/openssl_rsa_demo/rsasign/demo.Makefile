include ../Inc.mk

OBJS    =  rsa_demo.o Base64.o
BIN_SVR =  rsa_demo 

BINS = $(BIN_SVR)

all: $(BINS)

$(BIN_SVR):$(OBJS)
	$(CXX) -o $@ $^ $(LIBPATH) $(LIBS)
	@echo "build $(BIN_SVR) OK!"

install:
	@echo "nothing to install"

clean:
	rm -f $(OBJS) $(BINS)

