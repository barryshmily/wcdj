
CXX = g++
CFLAGS = -Wall -Os -pipe

DEP = ../../../src/dep
OAUTH_PATH = $(DEP)/liboauth/osx64_gcc421/liboauth-1.0.3


INCLUDE = -I$(OAUTH_PATH)/include
LIBS = -L$(OAUTH_PATH)/lib -loauth
OBJS = test_urlencode.o

DEMO = test_urlencode

all: $(DEMO)

$(DEMO): $(OBJS)
	$(CXX) -o $@ $^ $(LIBS)
	@echo "build $(DEMO) ok!"

clean:
	-rm -f $(OBJS) $(DEMO)


%.o: %.cpp
	$(CXX) $(CFLAGS) $(INCLUDE) -c $<


