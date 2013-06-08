/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * GeneralAgentTcpSocketServer.cpp - _explain_
 *
 * Copyright (C) 2011 ezlibs.com, All Rights Reserved.
 *
 * $Id: GeneralAgentTcpSocketServer.cpp 0001 2012-04-06 09:58:37Z WuJunjie $
 *
 *  Explain:
 *     -tcp������ʵ��-
 *
 *  Update:
 *     2011-10-13 22:00:11 WuJunjie Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#include "CommonInclude.h"
#include <string.h>

#include "GeneralAgentTcpSocketServer.h"

CGeneralAgentTcpSocketServer::CGeneralAgentTcpSocketServer(ISocketHandler& h, std::string strSocketName)
        :CGeneralAgentTcpSocket(h, strSocketName)
{
    SetDeleteByHandler();
}

void CGeneralAgentTcpSocketServer::OnAccept()
{
    printf("CGeneralAgentTcpSocketServer::OnAccept:%s:%d\n", GetRemoteAddress().c_str(), GetRemotePort());
}
void CGeneralAgentTcpSocketServer::OnDisconnect()
{
    printf("CGeneralAgentTcpSocketServer::OnDisconnect()\n");
}

void CGeneralAgentTcpSocketServer::OnCommand(char *pCmdData
        , unsigned int ulCmdDataLen)
{
    DBG_CODE( printf("CGeneralAgentTcpSocketServer::OnCommand, len:%d\n", ulCmdDataLen); );
    // �����ã� �ر��Լ�
    if (pCmdData && ulCmdDataLen==1 && *pCmdData=='q')
    {
        SetCloseAndDelete();

        DBG_CODE( printf("CGeneralAgentTcpSocketServer::OnCommand q, SetCloseAndDelete\n"); );
    }

    // �����ã� echo
    if (pCmdData && ulCmdDataLen==1 && *pCmdData=='e')
    {
        char strEchoBuf[128];
        sprintf(strEchoBuf, "CGeneralAgentTcpSocketServer echo,cur time:%ld\n", time(NULL));
        SendBuf(strEchoBuf, strlen(strEchoBuf)+1 );
    }
    //////////
}

#ifdef USE_DATAPARSE_EXAMPLE
// ͨ��ͷ pCmd
// ���� pData�� ���ȼ� pCmd->u8DataLen
void CGeneralAgentTcpSocketServer::OnCommand_WifiAgent(WIFI_AGENT_PROTOCOL_HEAD *pCmd, unsigned char *pData)
{
    CGeneralAgentTcpSocket::OnCommand_WifiAgent(pCmd, pData);
    printf("CGeneralAgentTcpSocketServer::OnCommand_WifiAgent\n");
}
#endif
