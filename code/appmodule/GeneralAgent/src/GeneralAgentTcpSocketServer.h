/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * GeneralAgentTcpSocketServer.h - _explain_
 *
 * Copyright (C) 2011 ezlibs.com, All Rights Reserved.
 *
 * $Id: GeneralAgentTcpSocketServer.h 0001 2012-04-06 09:58:28Z WuJunjie $
 *
 *  Explain:
 *     -tcp������ʵ��-
 *
 *  Update:
 *     2011-10-13 22:00:11 WuJunjie Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
#ifndef _GENERALAGENTTCPSOCKETSERVER_H
#define _GENERALAGENTTCPSOCKETSERVER_H

#include "GeneralAgentTcpSocket.h"
#include <ISocketHandler.h>

class CGeneralAgentTcpSocketServer : public CGeneralAgentTcpSocket
{
public:
	CGeneralAgentTcpSocketServer(ISocketHandler& , std::string strSocketName="GeneralAgentTcpSocketServer");
	
	virtual void OnAccept();
	virtual void OnDisconnect();
	virtual void OnCommand(char *pCmdData, unsigned int ulCmdDataLen);
#ifdef EXAMPLE_DATAPARSE_EXAMPLE
	// ͨ��ͷ pCmd
	// ���� pData�� ���ȼ� pCmd->u8DataLen
	virtual void OnCommand_WifiAgent(WIFI_AGENT_PROTOCOL_HEAD *pCmd, unsigned char *pData);
#endif
};

#endif //_GENERALAGENTTCPSOCKETSERVER_H
