#!/bin/bash

fd_limit=`ulimit -n`
echo "current fd limit is $fd_limit"
[ "$fd_limit" -lt 1024 ] && echo "change fd limit to 1024" && ulimit -n 1024
[ $? != 0 ] && echo "change err" || echo "change ok"



