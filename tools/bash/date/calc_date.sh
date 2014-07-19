#/bin/sh

ts=`date +%s`
echo "current ts[$ts]"

time_readalbe=`date -d@"$ts"`
echo "time_readalbe[$time_readalbe]"

echo "input seconds: "
read sec
date -d"1970-01-01 UTC $sec seconds" +"%Y-%m-%d %T %z"



