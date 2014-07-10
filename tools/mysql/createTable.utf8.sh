#!/bin/sh
#create order table
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

echo "`date +%y-%m-%d-%X`:begin to create... " 

printf "
use lxpay_order;
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
    CREATE TABLE t_order_state_$iTable (
    FOrderId             varchar(50)    NOT NULL COMMENT 'OrderId',
    FStatus              int(11)        NOT NULL default '0' COMMENT '1:unpay  2:paysuccess 3:payerror 4:payconfirm 5:paycanel 6:verifysuccess',
    FPortalSerialNo      varchar(50)    NOT NULL default ''  COMMENT 'PortalSerialNo',
	FOrderTime           varchar(30)    NOT NULL default '0000-00-00 00:00:00' COMMENT 'OrderTime',
	FProvideTime         varchar(30)    NOT NULL default '0000-00-00 00:00:00' COMMENT 'ProvideTime',
    FQQUin               varchar(24)    NOT NULL default ''  COMMENT 'QQUin',
    FPayUin              varchar(255)   NOT NULL default ''  COMMENT 'PayUin',
    FProvideUin          varchar(255)   NOT NULL default ''  COMMENT 'ProvideUin',
    FAppId               varchar(24)    NOT NULL default ''  COMMENT 'Appid',
    FSpoaId              varchar(24)    NOT NULL default ''  COMMENT 'SerivceCode',
    FType                int(11)        NOT NULL default '0' COMMENT 'Type',
    FPayChannel          varchar(24)    NOT NULL default ''  COMMENT 'PayChannel',
    FPayChannelSubId     varchar(24)    NOT NULL default ''  COMMENT 'PayChannelSubId',
    FPayItem             varchar(2048)  NOT NULL default ''  COMMENT 'PayItem',
    FUserIp              varchar(64)    NOT NULL default ''  COMMENT 'UserIp',
    FPayAmt              int(11)        NOT NULL default '0' COMMENT 'PayAmt',
    FOrigPayAmt          int(11)        NOT NULL default '0' COMMENT 'OrigPayAmt',
    FPayInfo             varchar(2048)  NOT NULL default ''  COMMENT 'PayInfo', 
    FPayRemark           varchar(2048)  NOT NULL default ''  COMMENT 'PayRemark', 
    FExtend              varchar(2048)  NOT NULL default ''  COMMENT 'Extend', 
    FSource              varchar(24)    NOT NULL default ''  COMMENT 'Source',
    FCurrencyAmt         varchar(16)    NOT NULL default ''  COMMENT 'CurrencyAmt',
    FCurrencyType        varchar(16)    NOT NULL default ''  COMMENT 'CurrencyType',

    FUserOrderId         varchar(50)    NOT NULL default ''  COMMENT 'user order id',
    FUserOrderMsg        varchar(2048)  NOT NULL default ''  COMMENT 'user order msg',
    FUserProvideMsg      varchar(1024)  NOT NULL default ''  COMMENT 'user provide msg',
    masterver            int unsigned   NOT NULL default 100 COMMENT 'hold inner field',
    slavever             int unsigned   NOT NULL default 100 COMMENT 'hold inner field',
    FUpdateTime          timestamp      NOT NULL default CURRENT_TIMESTAMP on update CURRENT_TIMESTAMP COMMENT 'LastModifyTime',
    PRIMARY KEY (FOrderId)
    ) ENGINE=InnoDB DEFAULT CHARSET=utf8;

    CREATE TABLE t_order_content_$iTable (
    FOrderId             varchar(50)    NOT NULL COMMENT 'OrderId',
    FPortalSerialNo      varchar(50)    NOT NULL default ''  COMMENT 'PortalSerialNo',
    FQQUin               varchar(24)    NOT NULL default ''  COMMENT 'QQUin',
    FPayUin              varchar(255)   NOT NULL default ''  COMMENT 'PayUin',
    FProvideUin          varchar(255)   NOT NULL default ''  COMMENT 'ProvideUin',
    FAppId               varchar(24)    NOT NULL default ''  COMMENT 'Appid',
    FSpoaId              varchar(24)    NOT NULL default ''  COMMENT 'SerivceCode',
    FType                int(11)        NOT NULL default '0' COMMENT 'Type',
    FPayChannel          varchar(24)    NOT NULL default ''  COMMENT 'PayChannel',
    FPayChannelSubId     varchar(24)    NOT NULL default ''  COMMENT 'PayChannelSubId',
    FPayItem             varchar(2048)  NOT NULL default ''  COMMENT 'PayItem',
    FUserIp              varchar(64)    NOT NULL default ''  COMMENT 'UserIp',
    FPayAmt              int(11)        NOT NULL default '0' COMMENT 'PayAmt',
    FOrigPayAmt          int(11)        NOT NULL default '0' COMMENT 'OrigPayAmt',
    FUserData            MEDIUMTEXT     NOT NULL default ''  COMMENT 'UserData16M', 
    FPayInfo             varchar(2048)  NOT NULL default ''  COMMENT 'PayInfo', 
    FPayRemark           varchar(2048)  NOT NULL default ''  COMMENT 'PayRemark', 
    FExtend              varchar(2048)  NOT NULL default ''  COMMENT 'Extend', 
    FSource              varchar(24)    NOT NULL default ''  COMMENT 'Source',
    FCurrencyAmt         varchar(16)    NOT NULL default ''  COMMENT 'CurrencyAmt',
    FCurrencyType        varchar(16)    NOT NULL default ''  COMMENT 'CurrencyType',

    FUserOrderId         varchar(50)    NOT NULL default '' COMMENT 'user order id',
    FUserOrderMsg        varchar(2048)  NOT NULL default '' COMMENT 'user order msg',
    FUserProvideMsg      varchar(1024)  NOT NULL default '' COMMENT 'user provide msg',
    FReserve1            varchar(1024)  NOT NULL default '' COMMENT 'reserve1',
    FReserve2            varchar(1024)  NOT NULL default '' COMMENT 'reserve2',
    FReserve3            varchar(1024)  NOT NULL default '' COMMENT 'reserve3',
    FOrderTime           datetime       NOT NULL default '0000-00-00 00:00:00' COMMENT 'order time',
    FProvideTime         datetime       NOT NULL default '0000-00-00 00:00:00' COMMENT 'provide time',
    FUpdateTime          timestamp      NOT NULL default CURRENT_TIMESTAMP on update CURRENT_TIMESTAMP COMMENT 'LastModifyTime',
    PRIMARY KEY (FOrderId)
    ) ENGINE=InnoDB DEFAULT CHARSET=utf8
    " | mysql $Info --default-character-set=utf8 lxpay_order 

    echo "create table $iTable "

    iDTable=$(($iDTable+1))
done

echo "`date +%y-%m-%d-%X`:end to create........" 
