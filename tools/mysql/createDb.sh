#!/bin/sh
#create db 
echo ""
echo ""
if [ $# -lt 1 ] 
then
	echo "usage : $0 DBInfo"
	echo "Sample: $0 \"-ulxpay -h172.27.198.179 -plxpay -P3306\""
	exit 1  
fi

Info=$1

echo "`date +%y-%m-%d-%X`:begin to create... " 

printf "
create database lxpay_order;
" | mysql $Info

echo "`date +%y-%m-%d-%X`:end to create........" 
