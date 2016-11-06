C_ARGS    = -g  -Wall
BINARY    = submit

CXX = g++
CC = gcc

all:	$(BINARY)

.cpp.o:
	$(CXX) $(C_ARGS) -c $^ 

.c.o:
	$(CC) $(C_ARGS) -c $^ 

submit: submit.o
	$(CXX) $(C_ARGS) -o $@ $^

clean:
	rm *.o $(BINARY)

