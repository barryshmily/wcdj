include ../../Inc.mk


LIB_IPC_FIFO     = ../../../lib/libipcfifo.a
OBJS_IPC_FIFO    = ipc_fifo.o

LIBS = $(LIB_IPC_FIFO) 
OBJS = $(OBJS_IPC_FIFO) 

all: $(LIBS)

$(LIB_IPC_FIFO): $(OBJS_IPC_FIFO)
	$(AR) $(AROPT) $@ $? 
	$(RANLIB) $@


install:
	@echo "nothing to install"
		
clean:
	rm -f $(OBJS) $(LIBS)

