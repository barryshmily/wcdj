#!/bin/bash

echo -n 'aaa' | md5sum | cut -d ' ' -f1
exit 0
