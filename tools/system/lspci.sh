#!/bin/bash

:<< WCDJ
linux

$lspci | grep Ethernet --color
01:00.0 Ethernet controller: Intel Corporation 82580 Gigabit Network Connection (rev 01)
01:00.1 Ethernet controller: Intel Corporation 82580 Gigabit Network Connection (rev 01)

显示是Intel的千兆网卡, 查看当前网卡的流量可以通过sar命令, 例如:
sar -n DEV 1 4
当千兆网卡跑满时, 例如如下信息: (压测时的数据)
Average:        IFACE   rxpck/s   txpck/s    rxkB/s    txkB/s   rxcmp/s   txcmp/s  rxmcst/s
Average:           lo      0.00      0.00      0.00      0.00      0.00      0.00      0.00
Average:         eth0      0.00      0.00      0.00      0.00      0.00      0.00      0.00
Average:         eth1 175921.19 208184.25 117945.58  14249.93      0.00      0.00      0.00
Average:        tunl0      0.00      0.00      0.00      0.00      0.00      0.00      0.00
Average:         sit0      0.00      0.00      0.00      0.00      0.00      0.00      0.00
Average:      ip6tnl0      0.00      0.00      0.00      0.00      0.00      0.00      0.00

计算下可以验证已经到达了网卡的最大吞吐量, 注意网卡的单位是小b, 即千兆网卡是1000Mb = 125MB
gerryyang@mba:system$echo "(117945.58 + 14249.93) / 1024 * 8" | bc
1032

WCDJ

lspci | grep Ethernet --color


