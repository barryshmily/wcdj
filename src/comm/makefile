include ../Inc.mk

INCLUDE = -I../../inc
LIBPATH = 

LIB_SV_QUEUE     = ../../lib/libsvqueue.a
OBJS_SV_QUEUE    = CSV_message_queue.o

LIB_COMM_TOOLS   = ../../lib/libcommtools.a
OBJS_COMM_TOOLS  = CAppConfig.o COption.o CParsepara.o

LIBS = $(LIB_SV_QUEUE) $(LIB_COMM_TOOLS)
OBJS = $(OBJS_SV_QUEUE) $(OBJS_COMM_TOOLS)

all: $(LIBS)

$(LIB_SV_QUEUE): $(OBJS_SV_QUEUE)
	$(AR) $(AROPT) $@ $? 
	$(RANLIB) $@

$(LIB_COMM_TOOLS): $(OBJS_COMM_TOOLS)
	$(AR) $(AROPT) $@ $?
	$(RANLIB) $@


install:
	@echo "nothing to install"
		
clean:
	rm -f $(OBJS) $(LIBS)
