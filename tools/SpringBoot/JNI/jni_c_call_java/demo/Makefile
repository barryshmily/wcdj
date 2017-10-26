all: run

Sample2.class: Sample2.java
	$(JAVA_HOME)/bin/javac Sample2.java

Sample2: Sample2.c
	gcc -o Sample2 \
	-L $(JAVA_HOME)/jre/lib/amd64/server/ \
	-ljvm \
	-I $(JAVA_HOME)/include/ \
	-I $(JAVA_HOME)/include/linux/ \
	Sample2.c

run: Sample2.class Sample2
	export LD_LIBRARY_PATH="$(JAVA_HOME)/jre/lib/amd64/server/" && ./Sample2

clean:
	rm -f Sample2.class Sample2
