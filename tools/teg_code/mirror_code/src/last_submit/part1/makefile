CC     = gcc
CXX    = g++
RANLIB = ranlib
AR     = ar
AROPT  = -scurv

CFLAGS  =  -Wall -g -O2 -pipe -D_REENTRANT -Wno-sign-compare -Wno-unused
INCLUDE =  -I./
LIBPATH =  -L./

DEPS = $(patsubst %.o, .%.d, $(OBJS))

LIBS   =
OBJS   =  submit.o
SUBMIT =  submit

BINS = $(SUBMIT)
all: $(BINS)

$(SUBMIT):$(OBJS)
	$(CXX) -o $@ $^ $(LIBPATH) $(LIBS)
	@echo "build $(SUBMIT) OK!"

install:
	@echo "nothing to install"

clean:
	rm -f $(OBJS) $(BINS) 

.%.d: %.cpp
	$(CC) $(INCLUDE) -MM $< > $@
	@$(CC) $(INCLUDE) -MM $< | sed s/"^"/"\."/  |  sed s/"^\. "/" "/  | \
		sed s/"\.o"/"\.d"/  >> $@
%.o: %.cpp 
	$(CXX) $(CFLAGS) $(INCLUDE) -c $<

.%.d: %.c
	$(CC) $(INCLUDE) -MM $< > $@
	@$(CC) $(INCLUDE) -MM $< | sed s/"^"/"\."/  |  sed s/"^\. "/" "/  | \
		sed s/"\.o"/"\.d"/  >> $@
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $<

