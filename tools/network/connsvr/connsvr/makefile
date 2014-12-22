CCX = g++
CCFLAGS = -Wall -g
INCLUDE = 
LFLAGS  = -lz

SUBDIRS = 

TARGET = connsvr

SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)

.PHONY :all
all :Build Link

.PHONY : Build
Build: $(OBJS)
%.o : %.cpp
	$(CCX) $(INCLUDE) -c $(CCFLAGS) $< -o $@

.PHONY :Link
Link: $(TARGET)
$(TARGET): $(OBJS) LIBRARYS
	$(CCX) $(OBJS) -o $@ $(INCLUDE) $(LFLAGS)
    
LIBRARYS:
	@ for subdir in $(SUBDIRS); do \
        (cd $$subdir && $(MAKE)); \
    done

.PHONY:clean
clean:
	$(RM) $(TARGET) $(OBJS)
	@ for subdir in $(SUBDIRS); do \
        (cd $$subdir && $(MAKE) clean); \
    done
