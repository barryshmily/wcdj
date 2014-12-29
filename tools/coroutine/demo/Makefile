
BIN = demo_loop_simple demo_loop_makecontext

all: $(BIN) 

demo_loop_simple: demo_loop_simple.c
	gcc -g -Wall -o $@ $^

demo_loop_makecontext: demo_loop_makecontext.c
	gcc -g -Wall -o $@ $^

clean:
	-rm $(BIN) 
