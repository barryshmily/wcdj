#!/bin/bash

# 查看某个进程的启动时间
ps -A -opid,stime,etime,args | grep portal_tdf
