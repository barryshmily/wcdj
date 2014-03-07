#!/bin/bash

./safe_wcdj_svr.sh start 

./safe_wcdj_proxy5.sh start > /dev/null
./safe_wcdj_proxy4.sh start > /dev/null
./safe_wcdj_proxy3.sh start > /dev/null
./safe_wcdj_proxy2.sh start > /dev/null
./safe_wcdj_proxy1.sh start > /dev/null

echo "going to start client"
sleep 3
./safe_wcdj_client.sh start > /dev/null

