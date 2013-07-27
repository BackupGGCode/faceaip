/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * GeneralAgentTcpSocketServer.h - _explain_
 *
 * Copyright (C) 2011 ezlibs.com, All Rights Reserved.
 *
 * $Id: GeneralAgentTcpSocketServer.h 0001 2012-04-06 09:58:28Z WuJunjie $
 *
 *  Explain:
 *     -Tcp�ͻ���ʵ��-
 *
 *  Update:
 *     2011-10-13 22:00:11 WuJunjie Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
//#include "CommonInclude.h"

#include <string.h>
#include "GeneralAgentTcpSocketConnector.h"
#include "../Logs.h"

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


CGeneralAgentTcpSocketConnector::CGeneralAgentTcpSocketConnector(ISocketHandler& h
        , std::string strSocketName
        , time_t Timeout
        , bool bReconn)
        :CGeneralAgentTcpSocket(h, strSocketName)
        , m_strSocketName(strSocketName)
        ,m_b_connected(false)
{
    // initial connection timeout setting
    SetConnectTimeout(5);

    SetDeleteByHandler();

    SetSockName(strSocketName);
}


CGeneralAgentTcpSocketConnector::~CGeneralAgentTcpSocketConnector()
{}

CGeneralAgentTcpSocketConnector *CGeneralAgentTcpSocketConnector::Reconnect()
{
    std::auto_ptr<SocketAddress> ad = GetClientRemoteAddress();
    CGeneralAgentTcpSocketConnector *p = new CGeneralAgentTcpSocketConnector(Handler(), m_strSocketName, m_ttTimeout, m_bReconn);
    p -> SetDeleteByHandler();

    p -> Open(*ad);
    Handler().Add(p);
    return p;
}


void CGeneralAgentTcpSocketConnector::OnConnectFailed()
{
    CGeneralAgentTcpSocketConnector *p = Reconnect();
    // modify connection timeout setting
    p -> SetConnectTimeout(3);
}


void CGeneralAgentTcpSocketConnector::OnDelete()
{
    if (m_b_connected)
    {
        Reconnect();
    }
}

void CGeneralAgentTcpSocketConnector::SetCloseAndDelete()
{
    CGeneralAgentTcpSocket::SetCloseAndDelete();
    m_b_connected = false;
}

void CGeneralAgentTcpSocketConnector::OnConnect()
{
    // do not modify, wujj 2012-4-9 15:31:44 begin
    m_b_connected = true;
    // do not modify, wujj 2012-4-9 15:31:44 end
    ////////// add your code here
    DBG(
        printf("CGeneralAgentTcpSocketConnector::OnConnect()\n");
    );
    LOG4CPLUS_INFO(LOG_PORTS, "CGeneralAgentTcpSocketConnector::OnConnect()");

}
void CGeneralAgentTcpSocketConnector::OnReconnect()
{
    printf("CGeneralAgentTcpSocketConnector::OnReconnect()\n");
}

void CGeneralAgentTcpSocketConnector::OnDisconnect()
{
    DBG(
        printf("CGeneralAgentTcpSocketConnector::OnDisconnect()\n");
    );
    LOG4CPLUS_INFO(LOG_PORTS, "CGeneralAgentTcpSocketConnector::OnDisconnect()");
}

void CGeneralAgentTcpSocketConnector::OnConnectTimeout()
{
    DBG(
        printf("CGeneralAgentTcpSocketConnector::OnConnectTimeout()\n");
    );
    LOG4CPLUS_INFO(LOG_PORTS, "CGeneralAgentTcpSocketConnector::OnConnectTimeout()");
}

void CGeneralAgentTcpSocketConnector::OnCommand(char *pCmdData
        , unsigned int ulCmdDataLen)
{
    DBG( printf("CGeneralAgentTcpSocketConnector::OnCommand, len:%d\n", ulCmdDataLen); );
    // �����ã� �ر��Լ�
    if (pCmdData && ulCmdDataLen==1 && *pCmdData=='q')
    {
        SetCloseAndDelete();
        m_b_connected = false;

        DBG( printf("CGeneralAgentTcpSocketConnector::OnCommand q, SetCloseAndDelete\n"); );
    }

    // �����ã� echo
    if (pCmdData && ulCmdDataLen==1 && *pCmdData=='e')
    {
        char strEchoBuf[128];
        sprintf(strEchoBuf, "CGeneralAgentTcpSocketConnector echo,cur time:%ld\n", time(NULL));
        SendBuf(strEchoBuf, strlen(strEchoBuf)+1 );
    }
    //////////
}

#ifdef USE_DATAPARSE_EXAMPLE
// ͨ��ͷ pCmd
// ���� pData�� ���ȼ� pCmd->u8DataLen
void CGeneralAgentTcpSocketConnector::OnCommand_WifiAgent(WIFI_AGENT_PROTOCOL_HEAD *pCmd, unsigned char *pData)
{
    CGeneralAgentTcpSocket::OnCommand_WifiAgent(pCmd, pData);
    printf("CGeneralAgentTcpSocketConnector::OnCommand_WifiAgent\n");
}
#endif

#include "../Solar.h"
#include "ConfigTcpSocketConnector.h"
// ����
void CGeneralAgentTcpSocketConnector::Update()
{
    CConfigTcpSocketConnector __cfg;
    __cfg.update();

    static unsigned int iLastHB=0;
    if (g_Solar.GetRunPeriod() >= iLastHB+__cfg.getConfig().CheckPeriod)
    {
        char strEchoBuf[128];
        // todo get sn
        sprintf(strEchoBuf, "[{\"cmd\":\"HB\",\"sn\":\"%d\"}]", (int)time(NULL));
        SendBuf(strEchoBuf, strlen(strEchoBuf)+1 );
        DBG(
            __fline;
            printf("SendBuf:[%s]\n", strEchoBuf);
        );
        iLastHB= g_Solar.GetRunPeriod();
    }
}

