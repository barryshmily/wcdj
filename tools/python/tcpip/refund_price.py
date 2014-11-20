#!/usr/bin/env python
#coding=utf-8

import xdrlib
import time
import socket
import random
import sys
import os
import struct


"""
还款批价请求DEMO:

SeqNoPP=2&CmdCode=PricingReq&InternalItem=&PayChannel=qqacct&PayChannelSubId=1&PayInfo=gerry test&PayItem=qpay*12.3*1&PayRemark=gerry test2&PayUin=154854427&PortalExtendField=pr%3D50%26uni_appid%3D1450001462%26uni_appamt%3D123%26app_type%3DQ%26device_type%3DM%26device_name%3Diphone6%26scence_city_code%3Dshenzhen%26qguid%3DNULL%26&PortalSerialNo=-APPDJ21985-20141119-2058578195&ProvideUin=154854427&QQuin=154854427&RealPayChannel=&ReqTime=1416401937&SerialNo=0&ServiceCode=-APPDJ21985&SessionType=&Source=100&UserIP=10.0.0.1&VarItem=
"""

ip   = '172.27.35.35'
port = 19010

for i in range(1):

    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect((ip, port))
    
    CmdCode             =     "PricingReq"
    InternalItem        =     ""
    PayChannel          =     "qqacct"
    PayChannelSubId     =     "1"
    PayInfo             =     "gerry test"
    PayItem             =     "qpay*12.3*1"
    PayRemark           =     "gerry test2"
    PayUin              =     "154854427"    
    PortalExtendField   =     "pr%3D50%26uni_appid%3D1450001462%26uni_appamt%3D123%26app_type%3DQ%26device_type%3DM%26device_name%3Diphone6%26scence_city_code%3Dshenzhen%26qguid%3DNULL%26"
    PortalSerialNo      =     "-APPDJ21985-20141119-2058578195"
    ProvideUin          =     "154854427"
    QQuin               =     "154854427"
    RealPayChannel      =     ""
    ReqTime             =     "1416401937"
    SeqNoPP             =     "2"
    SerialNo            =     "0"
    ServiceCode         =     "-APPDJ21985"
    SessionType         =     ""
    Source              =     "100"
    UserIP              =     "10.0.0.1"
    VarItem             =     ""


    req_content = """CmdCode=%s&InternalItem=%s&PayChannel=%s&PayChannelSubId=%s&PayInfo=%s&PayItem=%s&PayRemark=%s&PayUin=%s&PortalExtendField=%s&PortalSerialNo=%s&ProvideUin=%s&QQuin=%s&RealPayChannel=%s&ReqTime=%s&SeqNoPP=%s&SerialNo=%s&ServiceCode=%s&SessionType=%s&Source=%s&UserIP=%s&VarItem=%s""" % ( CmdCode,
            InternalItem,
            PayChannel,
            PayChannelSubId,
            PayInfo,
            PayItem,
            PayRemark,
            PayUin,
            PortalExtendField,
            PortalSerialNo,
            ProvideUin,
            QQuin,
            RealPayChannel,
            ReqTime,
            SeqNoPP,
            SerialNo,
            ServiceCode,
            SessionType,
            Source,
            UserIP,
            VarItem
            )

    req_len1_2b  = struct.pack('@h', len(req_content))
    req_len2_2b  = struct.pack('@h', 0)
    req_len      = req_len1_2b + req_len2_2b
    print "header[", repr(req_len), "]"

    req = req_len + req_content
    print "req[" , repr(req), "]"

    sock.send(req)

    buffer_size = 2048
    recvmsg = sock.recv(buffer_size)
    print "ans[" , repr(recvmsg), "]"

    sock.close()

