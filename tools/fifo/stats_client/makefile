include ./Inc.mk

LIB_STATS_CLIENT     = ./libstatsclient.a
OBJS_STATS_CLIENT    = ipc_fifo.o stats_client.o

LIBS = $(LIB_STATS_CLIENT) 
OBJS = $(OBJS_STATS_CLIENT) 

all: $(LIBS)

$(LIB_STATS_CLIENT): $(OBJS_STATS_CLIENT)
	$(AR) $(AROPT) $@ $? 
	$(RANLIB) $@

install:
	@echo "nothing to install"
		
clean:
	rm -f $(OBJS) $(LIBS)

