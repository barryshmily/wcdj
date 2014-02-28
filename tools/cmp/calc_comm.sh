#!/bin/bash

:<< WCDJ_COMMENT
This script is used to compare A and B file using comm tool
WCDJ_COMMENT


# init ur private files
A=
B=

# calc unique data in A
comm -12 <(sort A|uniq) <(sort B|uniq)

# calc unique data in B
comm -13 <(sort A|uniq) <(sort B|uniq)

# calc comm data in A and B
comm -12 <(sort A|uniq) <(sort B|uniq)

# calc data in A-B
comm -12 <(sort A|uniq) <(sort B|uniq) > result_comm.out
comm -23 <(sort A|uniq) <(sort result_comm.out|uniq) > A-B.out  

# calc data in B-A
comm -12 <(sort A|uniq) <(sort B|uniq) > result_comm.out
comm -23 <(sort B|uniq) <(sort result_comm.out|uniq) > B-A.out  

