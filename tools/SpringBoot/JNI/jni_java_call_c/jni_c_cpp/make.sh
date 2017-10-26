#!/bin/bash

javac HelloJNICpp.java

javah HelloJNICpp

gcc -fPIC -shared -o libhello.so HelloJNICpp.c HelloJNICppImpl.cpp -I$JAVA_HOME/include -I$JAVA_HOME/include/linux -lstdc++

java -Djava.library.path=. HelloJNICpp