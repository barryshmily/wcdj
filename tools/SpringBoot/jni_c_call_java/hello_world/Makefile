all: run

helloWorld.class: helloWorld.java
	$(JAVA_HOME)/bin/javac helloWorld.java

hello_world: hello_world.c
	gcc -o hello_world \
	-L $(JAVA_HOME)/jre/lib/amd64/server/ \
	-ljvm \
	-I $(JAVA_HOME)/include/ \
	-I $(JAVA_HOME)/include/linux/ \
	hello_world.c

run: helloWorld.class hello_world
	export LD_LIBRARY_PATH="$(JAVA_HOME)/jre/lib/amd64/server/"
	./hello_world

clean:
	rm -f helloWorld.class hello_world
