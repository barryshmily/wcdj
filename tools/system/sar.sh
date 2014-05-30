#!/bin/bash

:<< WCDJ
sar有很多用途，如果要来监控网络流量，使用下面的命令行方式：

sar -n DEV interval count

其中，interval是统计时间间隔，以秒为单位；count是总共统计几次，如果为0就不断的统计直到 Ctrl+C 打断，否则执行count次就退出。

比如：sar -n DEV 1 4
比如：sar -n DEV 10 0

IFACE：LAN接口
rxpck/s：每秒钟接收的数据包
txpck/s：每秒钟发送的数据包
rxbyt/s：每秒钟接收的字节数
txbyt/s：每秒钟发送的字节数

或者：
watch more /proc/net/dev 

WCDJ

sar -n DEV 1 4

