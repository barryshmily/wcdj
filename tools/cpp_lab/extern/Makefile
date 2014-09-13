# Makefile, wcdj

CC = gcc
CXX = g++
CFLAGS= -g -Wall -ansi

INCLUDE = . 
LIBPATH = 
PUBLIBS = 
LIBS = $(PUBLIBS)

BIN_SERVER = myapp
OBJS_SERVER = cppFile.o cFile.o

BINS = $(BIN_SERVER)
OBJS = $(OBJS_SERVER)

all: $(BIN_SERVER) 

$(BIN_SERVER):$(OBJS_SERVER)	
	$(CXX) -o $@ $^ $(LIBPATH) $(LIBS)

install:
	@echo "nothing to install"

clean:
	@-rm -f $(OBJS) $(BINS)
	@echo "clean over"
