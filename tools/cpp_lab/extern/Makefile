CC     =  gcc
CXX    =  g++
CFLAGS =  -g -Wall -Os

INCLUDE =  .
LIBPATH =
PUBLIBS =
LIBS    =  $(PUBLIBS)

BIN  =  demo
OBJS =  cppFile.o cFile.o

all: $(BIN) 

$(BIN): $(OBJS)
	$(CXX) -o $@ $^ $(LIBPATH) $(LIBS)

install:
	@echo "nothing to install"

clean:
	@-rm -f $(OBJS) $(BIN)
	@echo "clean over"

