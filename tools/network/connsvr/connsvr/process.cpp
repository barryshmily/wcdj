/** Copyright(C)  1998-2014,Tencent Co. Ltd.
* File name:    process.cpp
* @author:      jiwubu
* @version:     V1.0   2014/09/14
* Description: 	具体的协议处理
* Log:
*/

#include "process.h"

enum CMD_TYPE
{
    CMD_TEST                      = 0x2900, //测试协议
};

//将包添加到conn中
void send_client(struct conn* c, struct pbpack* pkt)
{
    PackToStr(pkt);

    append_pkt(c, GetPackLen(pkt) );

    return;
}

int process_test(struct conn* c, struct pbpack* pkt)
{
    const char* rcvbuf = GetDataPos(pkt);
    int size = GetDataSize(pkt);

    //从发送缓存分配内存
    int length = size + 128;

    //直接获取conn发送缓冲的指针，将发送数据直接填写到这个缓冲，减少数据拷贝
    char* sndbuf = resize_sbuf(c, length);

    struct pbpack* rpkt = CreatePack(sndbuf, length);

    SetCmd(rpkt, GetCmd(pkt));
    SetSeq(rpkt, GetSeq(pkt) );

    //添加数据
    AddAllData(rpkt, rcvbuf, size);

    send_client(c, rpkt);

    Destroy(rpkt);

    return 0;
}

//处理每种命令类型的数据包
int process_packet(struct conn* c, char* buf, int len)
{
    int iret = 0;
    struct pbpack* pkt = StrToPack(buf, len);

    if( NULL == pkt )
    {
        Err("[%s:%d] StrToPack return NULL", c->ip, c->port);

        return -1;
    }

    //获取到命令字
    unsigned int ulCmd = GetCmd(pkt);

    switch(ulCmd)
    {
        case CMD_TEST:
            iret = process_test(c, pkt);
            break;

        default:
            Err("[%s:%d] unknown protocol [%u]", c->ip, c->port, ulCmd);
            break;
    }

    Destroy(pkt);

    return iret;
}

