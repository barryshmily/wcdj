include ../../../src/Inc.mk

INCLUDE = -I.
LIBPATH = 

LIB_TLPI_COMM     = ../lib/libtlpicomm.a
OBJS_TLPI_COMM    = error_functions.o get_num.o


LIBS = $(LIB_TLPI_COMM) 
OBJS = $(OBJS_TLPI_COMM) 

all: $(LIBS)

$(LIB_TLPI_COMM): $(OBJS_TLPI_COMM)
	$(AR) $(AROPT) $@ $? 
	$(RANLIB) $@



install:
	@echo "nothing to install"
		
clean:
	rm -f $(OBJS) $(LIBS)
