#!/bin/bash


proc="portal_dispatch"
while :; do ps -eo pid,ni,pri,pcpu,psr,comm | grep "$proc"; sleep 1; done

