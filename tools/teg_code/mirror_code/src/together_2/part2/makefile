C_ARGS    = -g  -Wall
BINARY    = check

CXX = g++
CC = gcc

all:	$(BINARY)

.cpp.o:
	$(CXX) $(C_ARGS) -c $^ 

.c.o:
	$(CC) $(C_ARGS) -c $^ 

check: check.o
	$(CXX) $(C_ARGS) -o $@ $^

clean:
	rm *.o $(BINARY)


