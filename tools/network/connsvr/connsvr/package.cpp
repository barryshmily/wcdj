/** Copyright(C)  1998-2014,Tencent Co. Ltd.
* File name:    package.cpp
* @author:      jiwubu
* @version:     V1.0   2014/09/14
* Description: 	longpackage协议操作函数
* Log:
*/

#include "package.h"

void SetCmd(struct pbpack* pkt, __u32 ulCmd)
{
    ulCmd = htonl(ulCmd);
    memcpy(&pkt->buf[PACKAGE_CMD], &ulCmd, sizeof(__u32));

    return;
}

__u32 GetCmd(const struct pbpack* pkt)
{
    __u32 ulCmd;
    memcpy(&ulCmd, &pkt->buf[PACKAGE_CMD], sizeof(__u32) );
    ulCmd = ntohl(ulCmd);

    return ulCmd;
}

void SetSeq(struct pbpack* pkt, __u32 wSeq)
{
    wSeq = htonl(wSeq);
    memcpy(&pkt->buf[PACKAGE_SEQ], &wSeq, sizeof(__u32) );

    return;
}

__u32 GetSeq(const struct pbpack* pkt)
{
    __u32 wSeq;
    memcpy(&wSeq, &pkt->buf[PACKAGE_SEQ], sizeof(__u32) );
    wSeq = ntohl(wSeq);

    return wSeq;
}

bool AddAllData(struct pbpack* pkt, const char* buf, int len)
{
    if( NULL == buf || len <= 0 )
    {
        return false;
    }

    if( pkt->tail + len > pkt->total - 1 )
    {
        return false;
    }

    memcpy(&pkt->buf[pkt->tail], buf, len);

    pkt->tail += len;
    pkt->buf[pkt->tail] = EOT;

    return true;
}

int GetPackLen(struct pbpack* pkt)
{
    return pkt->tail + 1;
}

const char* GetDataPos(struct pbpack* pkt)
{
    return (char*)(pkt->buf + pkt->pos);
}

int GetDataSize(struct pbpack* pkt)
{
    return  pkt->tail - pkt->pos;
}

void Destroy(struct pbpack* pkt)
{
    if( NULL != pkt )
    {
        free(pkt);

        pkt = NULL;
    }

    return;
}

struct pbpack* CreatePack(char* buf, int len)
{
    struct pbpack* pkt = (struct pbpack*)malloc(sizeof(struct pbpack));

    if( NULL == pkt )
    {
        return NULL;
    }

    pkt->buf = (BYTE*)buf;
    pkt->total = len;
    pkt->pos = PACKAGE_BODY;
    pkt->tail = PACKAGE_BODY;

    pkt->buf[PACKAGE_SOH] = SOH;
    pkt->buf[pkt->tail] = EOT;

    len = htonl(PACKAGE_MIN_LEN);
    memcpy(&pkt->buf[PACKAGE_LEN], &len, sizeof(unsigned int));

    return pkt;
}

const char* PackToStr(struct pbpack* pkt)
{
    if( NULL == pkt )
    {
        return NULL;
    }

    __u32 ulLen;

    pkt->buf[PACKAGE_SOH] = SOH;
    pkt->buf[pkt->tail] = EOT;

    ulLen = htonl(pkt->tail + 1);
    memcpy(&pkt->buf[PACKAGE_LEN], &ulLen, sizeof(__u32) );

    return (char*)pkt->buf;
}

struct pbpack* StrToPack(char* buf, int len)
{
    struct pbpack* pkt = (struct pbpack*)malloc(sizeof(struct pbpack));

    if( NULL == pkt )
    {
        return NULL;
    }

    pkt->buf = (BYTE*)buf;
    pkt->total = len;
    pkt->pos = PACKAGE_BODY;
    pkt->tail = len - 1;

    if (pkt->buf[PACKAGE_SOH] != SOH ||
            pkt->buf[pkt->tail] != EOT )
    {
        free(pkt);

        return NULL;
    }

    return pkt;
}

