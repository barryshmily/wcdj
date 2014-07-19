#!/bin/sh

echo "off_iptables"
iptables -D INPUT 1
iptables -D OUTPUT 1
echo "ok"
