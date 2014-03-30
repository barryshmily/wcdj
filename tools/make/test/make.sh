#!/bin/bash

# This is a stupid way, so do not use this.

g++ -I./inc -c src/add.cpp -o ./tmp/add.o
g++ -I./inc -c src/sub.cpp -o ./tmp/sub.o
g++ -I./inc -c src/main.cpp -o ./tmp/main.o
g++ -o ./bin/main ./tmp/add.o ./tmp/sub.o ./tmp/main.o

echo "make over, but do not use this way!"
