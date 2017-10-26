#!/bin/bash

# Compile the Java code
javac Sample1.java

# Create the C/C++ header file
javah Sample1


# ok, c
#gcc -fPIC -shared -o libSample1.so Sample1.c -I$JAVA_HOME/include -I$JAVA_HOME/include/linux 

# ok, cpp
g++ -fPIC -shared -o libSample1.so Sample1.cpp -I$JAVA_HOME/include -I$JAVA_HOME/include/linux 

java -Djava.library.path=. Sample1