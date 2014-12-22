#ifndef PACKAGE_H_H
#define PACKAGE_H_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

typedef unsigned char   BYTE;
typedef unsigned int   __u32;
typedef unsigned short __u16;

#define PKG_HEAD_SIZE  5
#define MAX_PKT_SIZE   (1024 * 1024)

/*
 ******   协议格式    *********
 * unsigned char  SOH;     包头
 * unsigned int   len;     包长
 * unsigned int   cmd;     命令号
 * unsigned int   seq;     序列号
 * unsigned char  body[n]; 变长的包体
 * unsigned char  EOT;     包尾

   1     4    4    4    n      1
   SOH  len  cmd  seq  body    EOT
*/

//包头和包尾
#define SOH              0x28
#define EOT              0x29

//包中各个字段的位置
#define PACKAGE_SOH      0
#define PACKAGE_LEN      1
#define PACKAGE_CMD      5
#define PACKAGE_SEQ      9
#define PACKAGE_BODY     13

//包的默认size
#define PACKAGE_DEFAULT_SIZE   1024

//包的最小长度
#define PACKAGE_MIN_LEN  18

struct pbpack
{
    BYTE *buf;
    int pos;
    int tail;
    int total;
};


void SetCmd(struct pbpack* pkt, __u32 ulCmd);

__u32 GetCmd(const struct pbpack* pkt);

void SetSeq(struct pbpack* pkt, __u32 wSeq);

__u32 GetSeq(const struct pbpack* pkt);

bool AddAllData(struct pbpack* pkt, const char* buf, int len);

int GetPackLen(struct pbpack* pkt);

const char* GetDataPos(struct pbpack* pkt);

int GetDataSize(struct pbpack* pkt);

void Destroy(struct pbpack* pkt);

struct pbpack* CreatePack(char* buf, int len);

const char* PackToStr(struct pbpack* pkt);

struct pbpack* StrToPack(char* buf, int len);

#endif

