#!/bin/bash

errno="115"
find /usr/include/ -name "errno.h" | xargs grep "$errno"

