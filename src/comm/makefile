include ../Inc.mk

INCLUDE = -I../../inc
LIBPATH = 

LIB_SV_QUEUE  = ../../lib/libsvqueue.a
OBJS_SV_QUEUE = CSV_message_queue.o


LIBS = $(LIB_SV_QUEUE) 
OBJS = $(OBJS_SV_QUEUE) 

all: $(LIBS)

$(LIB_SV_QUEUE): $(OBJS_SV_QUEUE)
	$(AR) $(AROPT) $@ $? 
	$(RANLIB) $@

install:
	@echo "nothing to do"
		
clean:
	rm -f $(OBJS) $(LIBS)
