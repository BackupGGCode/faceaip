/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * GeneralAgentHttpSocketConnector.h - _explain_
 *
 * Copyright (C) 2011 ezlibs.com, All Rights Reserved.
 *
 * $Id: GeneralAgentHttpSocketConnector.h 0001 2012-04-06 09:58:28Z WuJunjie $
 *
 *  Explain:
 *     -Tcp�ͻ���ʵ��-
 *
 *  Update:
 *     2011-10-13 22:00:11 WuJunjie Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#ifndef _GENERALAGENTHTTPSOCKETCONNECTOR_H
#define _GENERALAGENTHTTPSOCKETCONNECTOR_H

#include "GeneralAgentTcpSocket.h"
#include <ISocketHandler.h>

#include <http_parser.h>

#define CGENERALAGENTHTTPKEEPCONNECTOR_SOCKET_NAME "GeneralAgentHttpKeepConnector"

class CGeneralAgentHttpSocketConnector : public TcpSocket
	//public CGeneralAgentTcpSocket
{
public:
    CGeneralAgentHttpSocketConnector(ISocketHandler&
                                    , std::string strSocketName=CGENERALAGENTHTTPKEEPCONNECTOR_SOCKET_NAME
                                                                , time_t Timeout= 5
                                                                                  , bool bReconn = false);
    ~CGeneralAgentHttpSocketConnector();

    // �� TcpSocket::OnRead(); �е���
    virtual void OnRawData(const char *buf,size_t len);

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
#ifdef EXAMPLE_DATAPARSE_EXAMPLE
    // ͨ��ͷ pCmd
    // ���� pData�� ���ȼ� pCmd->u8DataLen
    virtual void OnCommand_WifiAgent(WIFI_AGENT_PROTOCOL_HEAD *pCmd, unsigned char *pData);
#endif
    /////////////////////////////

private:
    CGeneralAgentHttpSocketConnector(const CGeneralAgentHttpSocketConnector& s) : TcpSocket(s)
    {} // copy constructor
    CGeneralAgentHttpSocketConnector& operator=(const CGeneralAgentHttpSocketConnector& )
    {
        return *this;
    } // assignment operator
    /** Create a new instance and reconnect */
    CGeneralAgentHttpSocketConnector *Reconnect();
    bool m_b_connected;

    ///////////
    std::string m_strSocketName;
    time_t m_ttTimeout;
    bool m_bReconn;
    
    /////////////
    http_parser *m_pHttpParserCli;
    http_parser_settings *m_pHttpParserSettings;
};

#endif // _GENERALAGENTHTTPSOCKETCONNECTOR_H
