
#CXXFLAGS = -g -O2 -Wall -fPIC -pipe 
#CXXFLAGS = -g -O2 -Wall -fPIC -pipe -DMACRO_PROC 
#CXXFLAGS = -g -O2 -Wall -fPIC -pipe -DMACRO_LOG
CXXFLAGS = -g -O2 -Wall -fPIC -pipe -DMACRO_RELEASE

CXXFLAGS += -Wno-unused-variable -Wno-unused-function -fno-strict-aliasing

OBJ_DIR = obj

OBJS = $(OBJ_DIR)/piece_def.o \
       $(OBJ_DIR)/evaluation.o \
       $(OBJ_DIR)/submit.o
 
BIN    = submit
CREATE_DIR = create_dir

INCLUDE += 
LIBS += 

all: clean $(CREATE_DIR) $(BIN)

create_dir:
	[ -d "$(OBJ_DIR)" ] || mkdir "$(OBJ_DIR)"

$(BIN): $(OBJS)
	$(CXX) -o $@ $^ $(LIBS)

clean:
	rm -rf $(OBJ_DIR) $(BIN)

$(OBJ_DIR)/%.o:%.cpp
	$(CXX) -o $(OBJ_DIR)/$*.o $(CXXFLAGS) $(INCLUDE) -c $<

$(OBJ_DIR)/%.o:%.c
	$(CC) -o $(OBJ_DIR)/$*.o $(CFLAGS) $(INCLUDE) -c $<