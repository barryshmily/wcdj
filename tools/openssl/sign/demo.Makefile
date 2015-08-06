include ./Inc.local.mk

RSA_OBJS =  rsa_demo.o Base64.o
RSA_BIN  =  rsa_demo

HMAC_OBJS =  hmac_demo.o Base64.o hmac_tools.o
HMAC_BIN  =  hmac_demo

BINS = $(RSA_BIN) $(HMAC_BIN)

all: $(BINS)

$(RSA_BIN): $(RSA_OBJS)
	$(CXX) -o $@ $^ $(LIBPATH) $(LIBS)
	@echo "build $(RSA_BIN) OK!"

$(HMAC_BIN): $(HMAC_OBJS)
	$(CXX) -o $@ $^ $(LIBPATH) $(LIBS)
	@echo "build $(HMAC_BIN) OK!"

install:
	@echo "nothing to install"

clean:
	rm -f $(RSA_OBJS) $(HMAC_OBJS) $(BINS)

