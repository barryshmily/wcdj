#!/bin/bash


# My E-mail
echo -e '#define cat(a,b) a##.b\n#define addr(a,b) a##@b\naddr(gerryyang,cat(tencent,com))'|gcc -E -xc - 2>/dev/null|tail -n 1      
