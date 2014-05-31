#!/bin/bash

:<< WCDJ
linux

$lspci | grep Ethernet --color
01:00.0 Ethernet controller: Intel Corporation 82580 Gigabit Network Connection (rev 01)
01:00.1 Ethernet controller: Intel Corporation 82580 Gigabit Network Connection (rev 01)
WCDJ

lspci | grep Ethernet --color


