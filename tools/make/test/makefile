# makefile

CXX = g++

INC = ./inc/
SRC = ./src/
BIN = ./bin/
TMP = ./tmp/

OBJ := $(addprefix $(TMP), \
	$(addsuffix .o, $(basename $(notdir $(wildcard src/*.cpp)))))

TARGET = $(BIN)main

CXXFLAG = -c -I$(INC) -MMD -MP -MF

DEP := $(wildcard $(addsuffix .d, $(OBJ)))

$(TARGET):$(OBJ)
	$(CXX) -o $@ $^

$(OBJ):$(TMP)%.o:$(SRC)%.cpp
	$(CXX) $(CXXFLAG) $@.d -o $@ $<

clean:
	-rm -f $(TMP)* $(BIN)*

-include $(DEP)

#-------------
# output
# gerryyang@mba:test$make
# g++ -c -I./inc/ -MMD -MP -MF tmp/add.o.d -o tmp/add.o src/add.cpp
# g++ -c -I./inc/ -MMD -MP -MF tmp/main.o.d -o tmp/main.o src/main.cpp
# g++ -c -I./inc/ -MMD -MP -MF tmp/sub.o.d -o tmp/sub.o src/sub.cpp
# g++ -o bin/main tmp/add.o tmp/main.o tmp/sub.o



