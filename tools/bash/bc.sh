#!/bin/bash

:<< WCDJ
use bc command to calc different base
WCDJ

# 10 -> 16
echo "ibase=10; obase=16; 9901" | bc

# 16 -> 10
echo "ibase=16; obase=10; 26AD" | bc

# 10 -> 2
echo "ibase=10; obase=2; 15" | bc

# 2 -> 10, err
#echo "ibase=2; obase=10; 110" | bc



