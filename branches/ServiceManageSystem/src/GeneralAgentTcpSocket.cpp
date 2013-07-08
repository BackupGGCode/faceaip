/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * GeneralAgentTcpSocket.cpp - _explain_
 *
 * Copyright (C) 2011 ezlibs.com, All Rights Reserved.
 *
 * $Id: GeneralAgentTcpSocket.cpp 0001 2012-04-06 09:58:37Z WuJunjie $
 *
 *  Explain:
 *     -tcp ͳһд������ͳһЭ�����-
 *
 *  Update:
 *     2011-10-13 22:00:11 WuJunjie Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/


#include <string.h> /* memset */
#include <str_opr.h> /* dump */

#include "GeneralAgentTcpSocket.h"

#ifndef _DEBUG_THIS
//    #define _DEBUG_THIS
#endif
#ifdef _DEBUG_THIS
	#define DEB(x) x
	#define DBG(x) x
#else
	#define DEB(x)
	#define DBG(x)
#endif

#ifndef __trip
	#define __trip printf("-W-%d::%s(%d)\n", (int)time(NULL), __FILE__, __LINE__);
#endif
#ifndef __fline
	#define __fline printf("%s(%d)--", __FILE__, __LINE__);
#endif

#define ARG_USED(x) (void)&x;

CGeneralAgentTcpSocket::CGeneralAgentTcpSocket(ISocketHandler& h, std::string strSocketName) : TcpSocket(h)
{
    // ��Handle�Լ�����
    SetDeleteByHandler();
    // ���ݲ�����
#ifdef USE_DATAPARSE_EXAMPLE

    memset(&m_headLastOn, 0, sizeof(WIFI_AGENT_PROTOCOL_HEAD));
#else

    DisableInputBuffer();
#endif //USE_DATAPARSE_EXAMPLE

    m_tOnData = 0;

    SetSockName(strSocketName);
}

#ifdef USE_DATAPARSE_EXAMPLE
// read����д������ط���Ϊ��ͳһЭ���������Ϊ���ۿͻ��˻��Ƿ�����
// һ��ϵͳ��Э����ϵӦ����һ���ģ� wujunjie 2012-4-9 15:35:31
void CGeneralAgentTcpSocket::OnRead()
{
    size_t n = ibuf.GetLength();

    //__fline;
    //printf("data len:%d, time(NULL) %d -m_tOnData %d\n", n, time(NULL), m_tOnData);

    // ����10s��������Ϊ ��Ч�� ����֮�� ��ֹ�ͻ��˳����ʱ���ʹ�������
    if (n>0
        && time(NULL)-m_tOnData > WIFI_AGENT_PROTOCOL_DATA_TIMEOUT)
    {
        __trip;
        printf("data timeout len:%d\n", n);

        ibuf.Remove(n);
        memset(&m_headLastOn, 0, sizeof(WIFI_AGENT_PROTOCOL_HEAD));
    }


    TcpSocket::OnRead();
    m_tOnData = time(NULL);
    n = ibuf.GetLength();

    //  ������ ��������ֱ��ȫ���ͷ�
    if (n>WIFI_AGENT_PROTOCOL_DATA_MAX_LEN)
    {
        __trip;
        printf("data too len:%d\n", n);

        ibuf.Remove(ibuf.GetLength());
        memset(&m_headLastOn, 0, sizeof(WIFI_AGENT_PROTOCOL_HEAD));
        return;
    }

    // ����ĸ���ʵ�ʶ�������
    unsigned char wifiAgentData[16];

    // �ȼ���ϴ��Ƿ���δ�������Э��
    // ��������u8DataLen = 0��Э�鱻���棬 ��Ϊ������Э��ֱ�Ӵ�����ˣ� �����ٵ�����
    if (m_headLastOn.u8DataLen > 0)
    {
        if (n >= m_headLastOn.u8DataLen)
        {
            ibuf.Read((char *)wifiAgentData, m_headLastOn.u8DataLen);

            OnCommand_WifiAgent(&m_headLastOn, wifiAgentData);
            memset(&m_headLastOn, 0, sizeof(WIFI_AGENT_PROTOCOL_HEAD));
        }
    }

    n = ibuf.GetLength();

    while (n >= sizeof(WIFI_AGENT_PROTOCOL_HEAD))
    {

        ibuf.Read((char *)&m_headLastOn, sizeof(WIFI_AGENT_PROTOCOL_HEAD));

        n = ibuf.GetLength();

        // ���Ȳ��㣬 �´δ���
        if (n < m_headLastOn.u8DataLen)
        {
            break;
        }

        if(m_headLastOn.u8DataLen>0)
        {
            ibuf.Read((char *)wifiAgentData, m_headLastOn.u8DataLen);
        }
        OnCommand_WifiAgent(&m_headLastOn, wifiAgentData);
        memset(&m_headLastOn, 0, sizeof(WIFI_AGENT_PROTOCOL_HEAD));

        n = ibuf.GetLength();
    }
    return;
}
#endif // USE_DATAPARSE_EXAMPLE

void CGeneralAgentTcpSocket::OnRawData(const char *buf,size_t len)
{
    m_tOnData = time(NULL);
DBG(
    __fline;
    printf("CGeneralAgentTcpSocket::OnRawData:%d\n", (int)len);

    dumpBuffer(stdout
               , (unsigned char *)buf, len,
               SHOW_ASCII | SHOW_BINAR | SHOW_HEXAD | SHOW_LINER);
);
    //////////
    //�����ݾ͵��ã� ��һ�ַ�ʽ
    OnCommand((char *)buf, (unsigned int)len);
    ///////////
}

void CGeneralAgentTcpSocket::OnCommand(char *pCmdData, unsigned int ulCmdDataLen)
{
    DBG( printf("CGeneralAgentTcpSocket::OnCommand, len:%d\n", ulCmdDataLen); );
}

#ifdef USE_DATAPARSE_EXAMPLE
void CGeneralAgentTcpSocket::OnCommand_WifiAgent(WIFI_AGENT_PROTOCOL_HEAD *pCmd, unsigned char *pData)
{
    DBG( printf("OnCommand_WifiAgent %d %d %d\n"
                     , pCmd->u8CmdOrStatus
                     , pCmd->u8AddOrCommand
                     , pCmd->u8DataLen); );
}
#endif
