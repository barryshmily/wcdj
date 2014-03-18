#/bin/sh

ts=`date +%s`
echo "current ts[$ts]"

echo "input seconds: "
read sec
date -d"1970-01-01 UTC $sec seconds" +"%Y-%m-%d %T %z"
