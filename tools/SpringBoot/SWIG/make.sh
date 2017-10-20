#!/bin/bash


gcc -fPIC -c example.c example_wrap.c -I$JAVA_HOME/include -I$JAVA_HOME/include/linux

gcc -shared example.o example_wrap.o -o libexample.so

javac main.java

java -Djava.library.path=. main