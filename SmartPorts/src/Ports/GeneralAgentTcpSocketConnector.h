/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * GeneralAgentTcpSocketConnector.h - _explain_
 *
 * Copyright (C) 2011 ezlibs.com, All Rights Reserved.
 *
 * $Id: GeneralAgentTcpSocketConnector.h 0001 2012-04-06 09:58:28Z WuJunjie $
 *
 *  Explain:
 *     -Tcp�ͻ���ʵ��-
 *
 *  Update:
 *     2011-10-13 22:00:11 WuJunjie Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#ifndef _GENERALAGENTTCPSOCKETCONNECTOR_H
#define _GENERALAGENTTCPSOCKETCONNECTOR_H

#include "GeneralAgentTcpSocket.h"
#include <ISocketHandler.h>


class CGeneralAgentTcpSocketConnector : public CGeneralAgentTcpSocket
{
public:
    CGeneralAgentTcpSocketConnector(ISocketHandler&
                                    , std::string strSocketName="GeneralAgentTcpSocketConnector"
                                                                , time_t Timeout= 5
                                                                                  , bool bReconn = false);
    ~CGeneralAgentTcpSocketConnector();

    void SetCloseAndDelete();
    void OnConnectFailed();
    void OnDelete();

    /////////////////////////////
    // �����������
    virtual void OnConnect();
    virtual void OnReconnect();
    virtual void OnDisconnect();
    virtual void OnConnectTimeout();
    virtual void OnCommand(char *pCmdData, unsigned int ulCmdDataLen);
#ifdef USE_DATAPARSE_EXAMPLE
    // ͨ��ͷ pCmd
    // ���� pData�� ���ȼ� pCmd->u8DataLen
    virtual void OnCommand_WifiAgent(WIFI_AGENT_PROTOCOL_HEAD *pCmd, unsigned char *pData);
#endif
// heartbeat etc ...
	    virtual void Update();

    /////////////////////////////

private:
    CGeneralAgentTcpSocketConnector(const CGeneralAgentTcpSocketConnector& s) : CGeneralAgentTcpSocket(s)
    {} // copy constructor
    CGeneralAgentTcpSocketConnector& operator=(const CGeneralAgentTcpSocketConnector& )
    {
        return *this;
    } // assignment operator
    /** Create a new instance and reconnect */
    CGeneralAgentTcpSocketConnector *Reconnect();
    bool m_b_connected;

    ///////////
    std::string m_strSocketName;
    time_t m_ttTimeout;
    bool m_bReconn;
    ///////////
};

#endif // _GENERALAGENTTCPSOCKETCONNECTOR_H
