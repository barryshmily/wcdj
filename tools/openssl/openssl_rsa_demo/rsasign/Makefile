include ../Inc.mk

LIB_PATH     =  ../../lib
LIB_RSASIGN  =  $(LIB_PATH)/librsasign.a
OBJS_RSASIGN =  Base64.o rsa_tools.o

LIBS = $(LIB_RSASIGN) 
OBJS = $(OBJS_RSASIGN) 


all: $(LIBS)

$(LIB_RSASIGN): $(OBJS_RSASIGN)
	$(AR) $(AROPT) $@ $? 
	$(RANLIB) $@


install:
	@echo "nothing to install"
		
clean:
	rm -f $(OBJS) $(LIBS)

