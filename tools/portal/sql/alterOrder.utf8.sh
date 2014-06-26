#!/bin/sh
# alter hfpay order table
# add a new column to existed table

echo ""
echo ""
if [ $# -lt 3 ] 
then
    echo "usage : $0 BeginID EndID DBInfo"
    echo "Sample: $0 0 999 \"-uroot -h172.27.198.179 -p123456 -P3306\""
    exit 1  
fi

BeginID=$1
EndID=$2
Info=$3

echo "`date +%y-%m-%d-%X`:begin to alter... " 

printf "
use hfpay_order;
" | mysql $Info

iDTable=$BeginID
while test $iDTable -le $EndID; do    
    if [ $iDTable -lt 10 ];then
        iTable=`printf "00%d " $iDTable`
    elif [ $iDTable -lt 100 ];then
        iTable=`printf "0%d " $iDTable`
    else 
        iTable=`printf "%d " $iDTable`
    fi

    printf "
    ALTER TABLE t_order_content_$iTable ADD FSource varchar(24) NOT NULL default ''  COMMENT 'Source' AFTER FExtend
    " | mysql $Info --default-character-set=utf8 hfpay_order 

    echo "alter table $iTable "

    iDTable=$(($iDTable+1))
done

echo "`date +%y-%m-%d-%X`:end to alter........" 

