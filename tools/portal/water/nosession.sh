#!/bin/sh


# 统计2012-05-14的相关数据：

# 1，
# NoSession当天的请求量: 5718061
cat portal/portal_20120514* | grep "type=first" |grep "NoSession" | gawk 'BEGIN {RS="&"} $0 ~/^PortalSerialNo/ {print $1}' | gawk -F'=' '{print $2}' | sort -u | wc -l

# 2，
# NoSession的所有业务代码总数: 215
# NoSession的所有业务代码
cat portal/portal_20120514* | grep "type=first" |grep "NoSession" | gawk 'BEGIN {RS="&"} $0 ~/^ServiceCode/ {print $1}' | gawk -F'=' '{print $2}' | sort -u | wc -l
cat portal/portal_20120514* | grep "type=first" |grep "NoSession" | gawk 'BEGIN {RS="&"} $0 ~/^ServiceCode/ {print $1}' | gawk -F'=' '{print $2}' | sort -u > service_nosession.out

# 3，
# NoSession的所有渠道总数:
# NoSession的所有渠道
cat portal/portal_20120514* | grep "type=first" |grep "NoSession" | gawk 'BEGIN {RS="&"} $0 ~/^PayChannel/ {print $1}' | gawk -F'=' '{print $2}' | sort -u | wc -l
cat portal/portal_20120514* | grep "type=first" |grep "NoSession" | gawk 'BEGIN {RS="&"} $0 ~/^PayChannel/ {print $1}' | gawk -F'=' '{print $2}' | sort -u > channel_nosession.out



