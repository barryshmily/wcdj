#!/bin/sh

echo "on_iptables"
iptables -I OUTPUT -p all -d 172.27.198.179 -j DROP
echo "ok"
