include ../../Inc.mk


LIB_HTTP     = ../../../lib/libhttp.a
OBJS_HTTP    = http_handle.o

LIBS = $(LIB_HTTP) 
OBJS = $(OBJS_HTTP) 

all: $(LIBS)

$(LIB_HTTP): $(OBJS_HTTP)
	$(AR) $(AROPT) $@ $? 
	$(RANLIB) $@


install:
	@echo "nothing to install"
		
clean:
	rm -f $(OBJS) $(LIBS)

