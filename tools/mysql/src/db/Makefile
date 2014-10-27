include ../Inc.mk

LIB_PATH = ../lib
LIB_DB   = $(LIB_PATH)/libmydb.a 

INCLUDE += -I./ 

all: $(LIB_DB)

OBJ_LIMIT = dbbase.o db_handle.o
DEPS      = $(patsubst %.o, .%.d, $(OBJ_LIMIT) ) 


$(LIB_DB): $(OBJ_LIMIT) 
	$(AR) $(AROPT) $@ $?
	$(RANLIB) $@

install:
	echo "install. do nothing..."

clean: 
	-@rm *.o .*.d
	rm $(LIB_DB)

-include $(DEPS)

