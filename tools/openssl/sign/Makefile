include ../Inc.mk

LIB_PATH =  ../../lib

LIB_RSASIGN   =  $(LIB_PATH)/librsasign.a
OBJS_RSASIGN  =  Base64.o rsa_tools.o

LIB_HMACSIGN  =  $(LIB_PATH)/libhmacsign.a
OBJS_HMACSIGN =  Base64.o hmac_tools.o

LIBS = $(LIB_RSASIGN) $(LIB_HMACSIGN)
OBJS = $(OBJS_RSASIGN) $(OBJS_HMACSIGN)

BIN_PATH =  ../../bin
DEP_PATH =  ../../dep
LIB_PATH =  ../../lib

OPENSSL_PATH =  $(DEP_PATH)/openssl-1.0.2a

INCLUDE += -I$(OPENSSL_PATH)/include

all: $(LIBS)

$(LIB_RSASIGN): $(OBJS_RSASIGN)
	$(AR) $(AROPT) $@ $? 
	$(RANLIB) $@

$(LIB_HMACSIGN): $(OBJS_HMACSIGN)
	$(AR) $(AROPT) $@ $? 
	$(RANLIB) $@


install:
	@echo "nothing to install"
		
clean:
	rm -f $(OBJS) $(LIBS)

