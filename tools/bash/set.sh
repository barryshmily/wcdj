#!/bin/bash
set -e

url="http://127.0.0.1"
curl -sSL $url

echo "you can not arrive here because the top cmd is error when using set -e"

