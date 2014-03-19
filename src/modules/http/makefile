include ../../Inc.mk


LIB_SV_QUEUE     = ../../../lib/libhttp.a
OBJS_SV_QUEUE    = http_handle.o

LIBS = $(LIB_SV_QUEUE) 
OBJS = $(OBJS_SV_QUEUE) 

all: $(LIBS)

$(LIB_SV_QUEUE): $(OBJS_SV_QUEUE)
	$(AR) $(AROPT) $@ $? 
	$(RANLIB) $@


install:
	@echo "nothing to install"
		
clean:
	rm -f $(OBJS) $(LIBS)

