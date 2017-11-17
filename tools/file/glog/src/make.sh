#/bin/bash

GLOG_PATH=/data/home/gerryyang/LAMP/glog/install
g++ -Os -pipe -g -I${GLOG_PATH}/include -L${GLOG_PATH}/lib -lglog demo.cpp -o demo

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${GLOG_PATH}/lib
./demo
echo "over"

