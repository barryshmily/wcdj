#!/bin/bash

CMD="curl -v \"http://10.56.58.55:8081/example1?transname=demo&req=%7b%22k1%22%3a%22v1%22%2c%22k2%22%3a%22v2%22%7d\""

CNT=$1
if [[ "$CNT" -lt "1" ]]; then
	CNT=1
fi
echo "$CNT" process


for ((i=0; i<$CNT; i++))
do
	$CMD >> out& 2>&1
done

wait 

echo "over"


