#!/bin/bash

javac HelloJNI.java

javah HelloJNI

gcc -fPIC -shared -o libhello.so HelloJNI.c -I$JAVA_HOME/include -I$JAVA_HOME/include/linux

java -Djava.library.path=. HelloJNI